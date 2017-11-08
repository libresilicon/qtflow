#!/usr/bin/tclsh
#---------------------------------------------------------------------------
# place2def.tcl ---
#
# Read a GrayWolf .pl1 cell placement output file, and write a
# DEF file of placed COMPONENTS and PINS, and unrouted NETS.
# add ROW, TRACKS, and DIEAREA statements, as needed.
#
# This routine is like place2def2 but turns "twfeed" cells into
# fill cells instead of removing them.  It should be used when the
# standard cell set defines a filler cell.  "twfeed" connections in
# the .pin file are still ignored, as before.
#
# Modified 11/23/2013 to include support for the "decongest.tcl" script.
# That routine modifies instance names and widths to create a fake cell
# incorporating extra fill space that GrayWolf cannot optimize out.
# It is then the responsibility of place2def.tcl to split the cell back
# into the original cell plus the filler cell in front.
#---------------------------------------------------------------------------

set scaling 100.0

if {$argc < 2} {
   puts -nonewline stdout "Usage:  place2def <project_name> "
   puts stdout "<fill_cell> \[<num_layers>\] \[<scaling>\]"
   exit 0
}

puts stdout "Running place2def.tcl"

set topname [lindex $argv 0]
set fill_cell [lindex $argv 1]

set numlayers 0
if {$argc > 2} {
   set numlayers [lindex $argv 2]
}

if {$argc > 3} {
   set scaling [lindex $argv 3]
}

set pl1name ${topname}.pl1
set pl2name ${topname}.pl2
set pinname ${topname}.pin
set defname ${topname}.def
set obsname ${topname}.obs
set infoname ${topname}.info

if [catch {open $pl1name r} fpl1] {
   puts stderr "Error: can't open file $pl1name for input"
   return
}

if [catch {open $pl2name r} fpl2] {
   puts stderr "Error: can't open file $pl2name for input"
   return
}

if [catch {open $defname w} fdef] {
   puts stderr "Error: can't open file $defname for output"
   return
}

if [catch {open $obsname w} fobs] {
   puts stderr "Error: can't open file $obsname for output"
   return
}

#-----------------------------------------------------------------
# DEF file header
#-----------------------------------------------------------------

puts $fdef "VERSION 5.6 ;"
puts $fdef "NAMESCASESENSITIVE ON ;"
puts $fdef "DIVIDERCHAR \"/\" ;"
puts $fdef "BUSBITCHARS \"<>\" ;"
puts $fdef "DESIGN $topname ;"
puts $fdef "UNITS DISTANCE MICRONS $scaling ;"
puts $fdef ""

#set scaling [ expr $scaling / 10 ]

#-----------------------------------------------------------------
# Part 1:  Area and routing tracks
#-----------------------------------------------------------------

#-----------------------------------------------------------------
# Read the .pl2 file and get the full die area (components only)
#-----------------------------------------------------------------

# If a number of route layers wasn't specified in the project_vars.sh
# script, then use the number of layers defined in the config file.

if [catch {open $infoname r} finf] {
   puts stdout "Warning:  No file $infoname generated, using defaults."

   set pitchx 160
   set pitchy 200
   set offsetx 80
   set offsety 100
   set widthx 160
   set widthy 200

   set numlayers 3
   for {set i 1} {$i <= $numlayers} {incr i 2} {
      set metal${i}(name) metal${i}
      set metal${i}(pitch) $pitchx
      set metal${i}(orient) horizontal
      set metal${i}(offset) $offsetx
      set metal${i}(width) $widthx
      set metal${i}(skip) 1
   }
   for {set i 2} {$i <= $numlayers} {incr i 2} {
      set metal${i}(name) metal${i}
      set metal${i}(pitch) $pitchy
      set metal${i}(orient) vertical
      set metal${i}(offset) $offsety
      set metal${i}(width) $widthy
      set metal${i}(skip) 1
   }

} else {

   set i 0
   while {[gets $finf line] >= 0} {

      # Versions of qrouter since 1.2.3 provide version information, including
      # whether or not qrouter was compiled with Tcl/Tk and supports scripting

      if {![regexp {qrouter ([0-9]+)\.([0-9]+)\.([0-9]+)(.*)} $line lmatch \
		major minor subv rest]} {

         # Older versions of qrouter assumed a track offset of 1/2 track pitch.
         # Newer versions correctly take the offset from the LEF file and dump the
         # value to the info file.  Also the newer version records the track width,
         # although this is not used.

 	 if {[regexp {^[ \t]*([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)(.*)} \
			$line lmatch layer pitch offset width orient rest] <= 0} {
            regexp {^[ \t]*([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)} $line lmatch \
			layer pitch orient 
	    set offset [expr 0.5 * $pitch]
	    set width $pitch
	    set skip 1
         }

	 if {![regexp {[ \t]*([^ ]+)} $rest lmatch skip]} {
	    set skip 1
	 }

	 if {($pitch > 0) && ($width > 0)} {
            incr i
            set metal${i}(name) $layer
            set metal${i}(pitch) $pitch
            set metal${i}(orient) $orient
            set metal${i}(offset) $offset
            set metal${i}(width) $width
            set metal${i}(skip) $skip
	 } else {
	    puts stderr "Warning: metal route layer defined with zero width or pitch" 
	 }
      }
   }
   close $finf
   if {($numlayers == 0) || ($numlayers > $i)} {set numlayers $i}
}

# NOTE:  Treating all pitches the same for all layers in the same
# direction.  This is good for doing various calculations on cell
# and pin positions.  The track positions themselves will be placed
# according to the given route layer pitch.

if {$metal1(orient) == "horizontal"} {
   set pitchx [expr $scaling * $metal2(pitch)]
   set pitchy [expr $scaling * $metal1(pitch)]
   set offsetx [expr $scaling * $metal2(offset)]
   set offsety [expr $scaling * $metal1(offset)]
   set widthx [expr $scaling * $metal2(width)]
   set widthy [expr $scaling * $metal1(width)]
} else {
   set pitchx [expr $scaling * $metal1(pitch)]
   set pitchy [expr $scaling * $metal2(pitch)]
   set offsetx [expr $scaling * $metal1(offset)]
   set offsety [expr $scaling * $metal2(offset)]
   set widthx [expr $scaling * $metal1(width)]
   set widthy [expr $scaling * $metal2(width)]
}

# Add numlayers to the configuration file now that we know it

set halfpitchx [expr $pitchx / 2];
set halfpitchy [expr $pitchy / 2];
set halfwidthx [expr $widthx / 2];
set halfwidthy [expr $widthy / 2];

set xbot 1000
set ybot 1000 
set cellxbot 1000
set cellybot 1000 
set xtop 0
set ytop 0

while {[gets $fpl2 line] >= 0} {
   regexp {^[ \t]*([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+[^ ]+[ \t]+([^ ]+)} \
		$line lmatch rowidx llx lly urx ury align
   set pline [regexp {^[ \t]*twpin} $line lmatch]
   if {$pline > 0} {

      # Re-align pins to the nearest track pitch.  Set pin size to be
      # equal to a route width.  

      set pincx [expr ($llx + $urx) / 2]
      set pincy [expr ($lly + $ury) / 2]
      set pincx [expr $pincx - $halfpitchx - $offsetx]
      set pincy [expr $pincy - $halfpitchy - $offsety]
      set xgrid [expr 1 + floor($pincx / $pitchx)]
      set ygrid [expr 1 + floor($pincy / $pitchy)]
      set pincx [expr $xgrid * $pitchx + $offsetx]
      set pincy [expr $ygrid * $pitchy + $offsety]
      set llx [expr $pincx - $halfwidthx]
      set lly [expr $pincy - $halfwidthy]
      set urx [expr $pincx + $halfwidthx]
      set ury [expr $pincy + $halfwidthy]

   } else {
       set yvals($rowidx) [list $lly $ury]

       if {$llx < $cellxbot} {set cellxbot $llx}
       if {$lly < $cellybot} {set cellybot $lly}
   }
   if {$llx < $xbot} {set xbot $llx}
   if {$lly < $ybot} {set ybot $lly}
   if {$urx > $xtop} {set xtop $urx}
   if {$ury > $ytop} {set ytop $ury}

   if {$pline <= 0} {
      set corexbot $xbot
      set corextop $xtop
      set coreybot $ybot
      set coreytop $ytop
   }
}
close $fpl2

puts stdout "Limits: xbot = $xbot ybot = $ybot xtop = $xtop ytop = $ytop"

# Move cells down and left by the track offset.

set cellxbot [expr $cellxbot - $offsetx]
set cellybot [expr $cellybot - $offsety]

# Also adjust core values to put lower left corner at offsetx,offsety

set corextop [expr $offsetx + $corextop - $corexbot]
set coreytop [expr $offsety + $coreytop - $coreybot]
set corexbot $offsetx
set coreybot $offsety

puts stdout "Core values: $corexbot $coreybot $corextop $coreytop"
puts stdout "Offsets: $offsetx $offsety"

# Expand die dimensions by a half pitch in all directions, then snap to
# the track grid (assumes that the origin (0, 0) is a track position)

set diexbot [expr $xbot - $cellxbot - $halfpitchx]
set xgrid [expr floor($diexbot / $pitchx)]
set diexbot [expr $xgrid * $pitchx]
set dieybot [expr $ybot - $cellybot - $halfpitchy]
set ygrid [expr floor($dieybot / $pitchy)]
set dieybot [expr $ygrid * $pitchy]

set diextop [expr $xtop - $cellxbot + $halfpitchx]
set xgrid [expr floor($diextop / $pitchx)]
set diextop [expr $xgrid * $pitchx]
set dieytop [expr $ytop - $cellybot + $halfpitchy]
set ygrid [expr floor($dieytop / $pitchy)]
set dieytop [expr $ygrid * $pitchy]

puts $fdef "DIEAREA ( $diexbot $dieybot ) ( $diextop $dieytop ) ;"
puts $fdef ""

#------------------------------------------------------------------
# Write the tracks (we need to match the coordinate positions. . .)
#------------------------------------------------------------------

set width [expr $diextop - $diexbot]
set height [expr $dieytop - $dieybot]

for {set i 1} {$i <= $numlayers} {incr i} {
   set mname [subst \$metal${i}(name)]
   set mpitch [expr $scaling * [subst \$metal${i}(pitch)] ]
   if {[subst \$metal${i}(orient)] == "vertical"} {
      set xtracks [expr 1 + int($width / $mpitch)];
      puts $fdef "TRACKS X $diexbot DO $xtracks STEP $mpitch LAYER $mname ;"
   } else {
      set ytracks [expr 1 + int($height / $mpitch)];
      puts $fdef "TRACKS Y $dieybot DO $ytracks STEP $mpitch LAYER $mname ;"
   }
}
puts $fdef ""

# diagnostic
puts stdout "$numlayers routing layers"

set xtracks [expr int($width / $pitchx)];
set ytracks [expr int($height / $pitchy)];

if {$metal1(orient) == "horizontal"} {
   puts stdout \
	"$ytracks horizontal tracks from $dieybot to $dieytop step $pitchy (M1, M3, ...)"
   puts stdout \
	"$xtracks vertical tracks from $diexbot to $diextop step $pitchx (M2, M4, ...)"
} else {
   puts stdout \
	"$ytracks horizontal tracks from $dieybot to $dieytop step $pitchy (M2, M4, ...)"
   puts stdout \
	"$xtracks vertical tracks from $diexbot to $diextop step $pitchx (M1, M3, ...)"
}

# generate obstruction around pin areas, so these will not have vias
# dropped on top of the pin labels (convert values to microns)
set diexbot_um [expr ($diexbot - $pitchx) / $scaling]
set diextop_um [expr ($diextop + $pitchx) / $scaling]
set dieybot_um [expr ($dieybot - $pitchy) / $scaling]
set dieytop_um [expr ($dieytop + $pitchy) / $scaling]
set corexbot_um [expr $corexbot / $scaling]
set corextop_um [expr ($corextop + $pitchx) / $scaling]
set coreybot_um [expr $coreybot / $scaling]
set coreytop_um [expr ($coreytop + $pitchy) / $scaling]

#-----------------------------------------------------------------
# Finish route configuration file
#-----------------------------------------------------------------

# Obstruct all positions in metal1, unless there are only 2 routing layers defined.
# (This syntax is common to the Tcl and non-Tcl versions of qrouter)

if {$numlayers > 2} {
   set mname $metal1(name)
   # 1. Top
   puts $fobs \
	"obstruction $diexbot_um $coreytop_um $diextop_um $dieytop_um $mname"
   # 2. Bottom
   puts $fobs \
	"obstruction $diexbot_um $dieybot_um $diextop_um $coreybot_um $mname"
   # 3. Left
   puts $fobs \
	"obstruction $diexbot_um $dieybot_um $corexbot_um $dieytop_um $mname"
   # 4. Right
   puts $fobs \
	"obstruction $corextop_um $dieybot_um $diextop_um $dieytop_um $mname"
}

# Place obstructions along top and bottom, or left and right, on the layers that
# are between pin layers.

if {$metal2(orient) == "vertical"} {
   for {set i 3} {$i <= $numlayers} {incr i 2} {
      set mname [subst \$metal${i}(name)]
      # 1. Top
      puts $fobs \
	"obstruction $corexbot_um $coreytop_um $corextop_um $dieytop_um $mname"
      # 2. Bottom
      puts $fobs \
	"obstruction $corexbot_um $dieybot_um $corextop_um $coreybot_um $mname"
   }
   for {set i 2} {$i <= $numlayers} {incr i 2} {
      set mname [subst \$metal${i}(name)]
      # 3. Left
      puts $fobs \
	"obstruction $diexbot_um $coreybot_um $corexbot_um $coreytop_um $mname"
      # 4. Right
      puts $fobs \
	"obstruction $corextop_um $coreybot_um $diextop_um $coreytop_um $mname"
   }
} else {
   for {set i 3} {$i <= $numlayers} {incr i 2} {
      set mname [subst \$metal${i}(name)]
      # 1. Left
      puts $fobs \
	"obstruction $diexbot_um $coreybot_um $corexbot_um $coreytop_um $mname"
      # 2. Right
      puts $fobs \
	"obstruction $corextop_um $coreybot_um $diextop_um $coreytop_um $mname"
   }
   for {set i 2} {$i <= $numlayers} {incr i 2} {
      set mname [subst \$metal${i}(name)]
      # 3. Top
      puts $fobs \
	"obstruction $corexbot_um $coreytop_um $corextop_um $dieytop_um $mname"
      # 3. Bottom
      puts $fobs \
	"obstruction $corexbot_um $dieybot_um $corextop_um $coreybot_um $mname"
   }
}

# (test) generate blockages between power buses
# This gets the router stuck.  Too hard!  Limit to a small strip in
# the middle, enough to ensure placement of a substrate/well contact row.
# (This code not relevant to the OSU standard cell set, where power buses
# overlap.)

set i 1
while {![catch {set yvals($i)}]} {
   set oddrow [expr {$i % 2}]
   set y1 [lindex $yvals($i) 0]
#   if {$oddrow} {
#      set y2 [expr {$y1 + 196}]
#   } else {
#      set y2 [expr {$y1 + 84}]
#   }
   set y2 [expr {$y1 + 40}]
   set y3 [lindex $yvals($i) 1]
#   if {$oddrow} {
#      set y4 [expr {$y3 - 84}]
#   } else {
#      set y4 [expr {$y3 - 196}]
#   }
   set y4 [expr {$y3 - 40}]
#  puts $fobs "obstruction $corexbot $y1 $corextop $y2 $metal1(name)"
#  puts $fobs "obstruction $corexbot $y4 $corextop $y3 $metal1(name)"
   incr i
}

close $fobs

#-----------------------------------------------------------------
# Part 2:  Components and pins (placed)
#-----------------------------------------------------------------

#-----------------------------------------------------------------
# Pass number 1:  Read the .pl1 file and get the number of components and pins
#-----------------------------------------------------------------

set numpins 0
set numcomps 0
while {[gets $fpl1 line] >= 0} {
   # We only care about the first word on each line in this pass
   regexp {^[ \t]*([^ ]+)[ \t]+} $line lmatch instance
   if {[string equal -length 6 $instance twpin_]} {
      incr numpins
   } else {
      incr numcomps
      if [regexp {^[ \t]*[^ \t.]+\.([^ \t.]+)\.[^ \t.]+[ \t]+} $line lmatch fsize] {
	 if [regexp {[0-9]+X([0-9]+)} $fsize lmatch fmult] {
	    incr numcomps $fmult
	 } else {
	    incr numcomps
	 }
      }
   }
}
close $fpl1

# The .pl1 file always has components first, then pins

puts $fdef "COMPONENTS $numcomps ;"

#-----------------------------------------------------------------
# Pass number 2:  Re-read the .pl1 file
#-----------------------------------------------------------------

set fpl1 [open $pl1name r]
set lastrow -1

# Use layers 2 and 3 for pin placement, according to the routing
# orientation.  For now, we only allow pins on those layers
# (need to relax this requirement).  If only 2 routing layers are
# defined, use the metal1 layer for pins

if {$metal2(orient) == "vertical"} {
   set vlayer $metal2(name)
   set hskip $metal2(skip)
   if {$numlayers < 3} {
      set hlayer $metal1(name)
      set vskip $metal1(skip)
   } else {
      set hlayer $metal3(name)
      set vskip $metal3(skip)
   }
} else {
   set hlayer $metal2(name)
   set vskip $metal2(skip)
   if {$numlayers < 3} {
      set vlayer $metal1(name)
      set hskip  $metal1(skip)
   } else {
      set vlayer $metal3(name)
      set hskip  $metal3(skip)
   }
}

while {[gets $fpl1 line] >= 0} {
   # Each line in the file is <instance> <llx> <lly> <urx> <ury> <orient> <row>
   regexp \
   {^[ \t]*([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)} \
	$line lmatch instance llx lly urx ury orient row
   switch $orient {
      0 {set ostr "N"}
      1 {set ostr "FS"}
      2 {set ostr "FN"}
      3 {set ostr "S"}
      4 {set ostr "FE"}
      5 {set ostr "FW"}
      6 {set ostr "W"}
      7 {set ostr "E"}
   }
   set lastrow $row

   # Handle the "cells" named "twpin_*"

   if {[string equal -length 6 $instance twpin_]} {
      set labname [string range $instance 6 end]
      if {$row == -1 || $row == -2} {
	 set labtype $hlayer
	 set pinpitchx $pitchx
	 set pinpitchy [expr $vskip * $pitchy]
      } else {
	 set labtype $vlayer
	 set pinpitchx [expr $hskip * $pitchx]
	 set pinpitchy $pitchy
      }

      # Only deal with pin center position

      set pincx [expr ($llx + $urx) / 2]
      set pincy [expr ($lly + $ury) / 2]

      set pincx [expr $pincx - $cellxbot]
      set pincy [expr $pincy - $cellybot]
      
      set pinw [expr ($urx - $llx) / 2 ]
      set pinh [expr ($ury - $lly) / 2 ]

      # Reposition the pins to match track positions.  Make pins point labels.
      # Do NOT offset by (offsetx, offsety) because the offset has been applied
      # to the cell positions.  GrayWolf may violate track pitch, so record
      # positions used and avoid overlap.

      set pincx [expr $pincx - $halfpitchx]
      set pincy [expr $pincy - $halfpitchy]
      set xgrid [expr 1 + floor($pincx / $pinpitchx)]
      set ygrid [expr 1 + floor($pincy / $pinpitchy)]
      set llx [expr $xgrid * $pinpitchx]
      set lly [expr $ygrid * $pinpitchy]
      while {![catch {eval [subst {set posused(${llx},${lly})}]}]} {
         puts -nonewline stdout "Caught GrayWolf being bad:  "
         puts stdout "Pin $labname overlaps pin $posused(${llx},${lly})"
         if {$row == -1 || $row == -2} {
            set lly [expr $lly + $pinpitchy]
         } else {
            set llx [expr $llx + $pinpitchx]
         }
      }
      set posused(${llx},${lly}) $labname

      puts $fdef "- $labname + NET $labname"
      puts $fdef "  + LAYER $labtype ( -$pinw -$pinh ) ( $pinw $pinh )"
      puts $fdef "  + PLACED ( $llx $lly ) N ;"

   } else {

      if {[string equal -length 6 $instance twfeed]} {
         # Replace twfeed with name of filler cell
	 set cellname ${fill_cell}
      } else {

	 # Pull out the fill cell if one was spliced into
	 # the component name
	 if [regexp {([^ \t.]+)\.([^ \t.]+)\.([^ \t.]+)} $instance \
			lmatch fillcell fillwidth baseinst] {
	    if {![regexp {([0-9]+)X([0-9]+)} $fillwidth lmatch fillnom fmult]} {
	       set fillnom $fillwidth
	       set fmult 1
	    }
	    set cellname $fillcell
	    set instance $baseinst
	    set fillname $cellname
	    set llyoff [expr $lly - $cellybot]

	    for {set i 0} {$i < $fmult} {incr i} {

	       set llxoff [expr $llx - $cellxbot]
	       puts $fdef "- ${fillname}_$baseinst $cellname + PLACED ( $llxoff $llyoff ) $ostr ;"
	       incr numcomps -1
	       set llx [expr $llx + $fillnom]
	       set fillname "${cellname}$i"
	    }
	 }

         # Get cellname from instance name.
         regsub {(.+)_[\d]+$} $instance {\1} cellname
      }

      set llxoff [expr $llx - $cellxbot]
      set llyoff [expr $lly - $cellybot]
	
      puts $fdef "- $instance $cellname + PLACED ( $llxoff $llyoff ) $ostr ;"

      incr numcomps -1
      if {$numcomps == 0} {
	 puts $fdef "END COMPONENTS"
	 puts $fdef ""
	 puts $fdef "PINS $numpins ;"
      }
   }
}

close $fpl1

#---------------------------------------------------------------------------
# Part 3:  Nets (unrouted)
#---------------------------------------------------------------------------
# Read a GrayWolf .pin netlist file and produce a DEF netlist file for
# use with lithoroute.  The routine is the same as place2net2.tcl (for
# generating a netlist for the Magic interactive maze router), but the
# output format is DEF.
#---------------------------------------------------------------------------

set pinfile ${topname}.pin

if [catch {open $pinfile r} fpin] {
   puts stderr "Error: can't open file $pinfile for input"
   exit 0
}

#-----------------------------------------------------------------
# Pass #1: Parse the .pin file, and count the total number of nets
#-----------------------------------------------------------------

set numnets 0
set curnet {}
while {[gets $fpin line] >= 0} {
   regexp {^([^ ]+)[ \t]+} $line lmatch netname
   if {"$netname" != "$curnet"} {
      incr numnets
      set curnet $netname
   }
}

puts $fdef "END PINS"
puts $fdef ""
puts -nonewline $fdef "NETS $numnets "

#--------------------------------------------------------------
# Pass #2: Parse the .pin file, writing one line of output for
#	   each line of input.
#   While we're at it, enumerate the cells used.
#--------------------------------------------------------------

set curnet {}
set netblock {}
set newnet 0

set fpin [open $pinfile r]
set maclist {}

set lastinst ""
set lastpin ""
set skip 0

while {[gets $fpin line] >= 0} {
   # Each line in the file is:
   #     <netname> <subnet> <macro> <pinname> <x> <y> <row> <orient> <layer>
   regexp {^([^ ]+)[ \t]+(\d+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+([^ ]+)[ \t]+[^ ]+[ \t]+[^ ]+[ \t]+([^ ]+)} \
		$line lmatch netname subnet instance pinname px py layer
   if {"$netname" != "$curnet"} {
      set newnet 1
      set curnet $netname
      puts $fdef ";"
      puts $fdef "- $netname"
   }

   # Rip the filler cell name off the beginning of instances where
   # they were spliced in

   if [regexp {([^ \t.]+)\.([^ \t.]+)\.([^ \t.]+)} $instance \
			lmatch fillcell fillwidth baseinst] {
      set instance $baseinst
   }

   if {([string first twfeed ${instance}] == -1) &&
		([string first twfeed ${pinname}] != 0)} {
      if {[string first twpin_ ${instance}] == 0} { 
         if {$newnet == 0} {
	    puts $fdef ""	;# end each net component with newline
	 }
         puts -nonewline $fdef "  ( PIN ${pinname} ) "
	 set newnet 0
      } elseif {$instance != "PSEUDO_CELL"} {

	 # If pinname contains "bF$pin/", pin name follows this text
	 if {[regexp {.+bF\$pin/(.*)$} $pinname lmatch shortname]} {
	    set pinname $shortname
	 }

	 # Avoid output for redundant entries generated by graywolf for
	 # pins it wants to route through both top and bottom.
	 if {"${lastinst}" == "${instance}" && "${lastpin}" == "${pinname}"} {
	    set skip 1
	 } else {
	    set skip 0
	    set lastinst $instance
	    set lastpin $pinname
	 }

         if {$newnet == 0 && $skip == 0} {
	    puts $fdef ""	;# end each net component with newline
	 }
	 # Avoid output for redundant entries generated by graywolf for
	 # pins it wants to route through both top and bottom.
	 if {$skip == 0} {
            puts -nonewline $fdef "  ( ${instance} ${pinname} ) "
 	 }
	 set newnet 0
	 regexp {(.+)_[^_]+} $instance lmatch macro
	 lappend maclist $macro
      }
   }
}

puts $fdef ";"
puts $fdef "END NETS"
puts $fdef ""
puts $fdef "END DESIGN"

close $fpin
close $fdef

puts stdout "Done with place2def.tcl"
