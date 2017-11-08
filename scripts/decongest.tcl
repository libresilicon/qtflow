#!/usr/bin/tclsh
#
# Usage:
#	decongest.tcl <rootname> <leffile> <fillcell> [<scale> <offset>]
#
# Take information produced by qrouter's "congested" command,
# and use it to add fill-cell padding to the most congested
# instances in the .cel file. 
#
# or
#	decongest.tcl <rootname> <leffile> <fillcell> <density>
#
# Take the existing <rootname>.cel file for graywolf input,
# and pad the area out with randomly-assigned fill cells
# to reduce the density of actively routed cells in the design.
#
# This procedure is necessary because GrayWolf does not
# have a way (apparently) to reduce internal density by
# adding spacers.  So we use feedback from the router to
# find targeted areas to add extra fill.  Since GrayWolf
# only looks at the contents of the .cel file, padding can
# be added by rewriting the cell width for specific cells
# to include the width of one or more filler cells.  By
# annotating the cell instance name, it becomes easy to
# add the fill cells into the DEF file after placement.
#
# "scale" gives a scalefactor which is the amount of
# increase in congestion that will cause an additional fill
# cell to be added.  Default is 0.1.  Scale will be adjusted
# upward according to the percentage of failing nets.

# "offset" is the amount of congestion in the .cinfo file
# that is the baseline for adding fill.  Should be equal to
# the minimum amount of congestion for which a fill cell is
# added.  Default is 0.6.
#
# "density" is a fraction of the area of the design that
# is made up of actively routed cells;  that is, the area
# given to fill cells is (1 - density).  It is used in place
# of "<scale> <offset>", and does not expect a ".cinfo" file
# of congestion information.  The fill cells will be randomly
# placed.  There is no default density.
#
#------------------------------------------------------------
# Written by Tim Edwards, November 23, 2013
#------------------------------------------------------------
# LEF dimensions are microns unless otherwise stated.
#------------------------------------------------------------

if {$argc != 3 && $argc != 4 && $argc != 5} {
   puts stderr "Bad argument list"
   puts stderr "Usage: decongest.tcl <rootname> <leffile> <fillcell> [<scale> <offset>]"
   puts stderr "or:    decongest.tcl <rootname> <leffile> <fillcell> <density>"
   exit 1
}

set units 100
set scale 0.1
set offset 0.5

set cellname [file rootname [lindex $argv 0]]

set celfile ${cellname}.cel
set annofile ${cellname}.acel
set cinfofile ${cellname}.cinfo
set lefname [lindex $argv 1]
set fillcell [lindex $argv 2]

if {$argc == 4} {
   set density [lindex $argv 3]
   set scale 0.0	;# scale 0 used to denote density planning
} elseif {$argc == 5} {
   set scale [lindex $argv 3]
   set offset [lindex $argv 4]
}

if [catch {open $lefname r} flef] {
   puts stderr "Error: can't open file $lefname for input"
   return
}

if [catch {open $celfile r} fcel] {
   puts stderr "Error: can't open file $celfile for input"
   return
}

if {$scale > 0.0} {
   if [catch {open $cinfofile r} finf] {
      puts stderr "Error: can't open file $cinfofile for input"
      return
   }
}

if [catch {open $annofile w} fanno] {
   puts stderr "Error: can't open file $annofile for output"
   return
}

#----------------------------------------------------------------
# Read through a LEF file section that we don't care about.
#----------------------------------------------------------------

proc skip_section {leffile sectionname} {
   while {[gets $leffile line] >= 0} {
      if [regexp {[ \t]*END[ \t]+(.+)[ \t]*$} $line lmatch sectiontest] {
         if {"$sectiontest" != "$sectionname"} {
            puts -nonewline stderr "Unexpected END statement $line "
            puts stderr "while reading section $sectionname"
         }
         break
      }
   }
}

#----------------------------------------------------------------
# Parse the macro contents of the LEF file and retain the information
# about cell size and pin positions.
#----------------------------------------------------------------

proc parse_macro {leffile macroname} {
   global $macroname units

   while {[gets $leffile line] >= 0} {
      if [regexp {[ \t]*SYMMETRY[ \t]+(.+)[ \t]*;} $line lmatch symmetry] {
         set ${macroname}(symmetry) $symmetry
      } elseif [regexp {[ \t]*ORIGIN[ \t]+(.+)[ \t]+(.+)[ \t]*;} $line lmatch x y] {
         set x [expr {int($x * $units + 0.5)}]
         set y [expr {int($y * $units + 0.5)}]
         set ${macroname}(x) $x
         set ${macroname}(y) $y
      } elseif [regexp {[ \t]*SIZE[ \t]+(.+)[ \t]+BY[ \t]+(.+)[ \t]*;} \
                        $line lmatch w h] {
         set w [expr {int($w * $units + 0.5)}]
         set h [expr {int($h * $units + 0.5)}]
         set ${macroname}(w) $w
         set ${macroname}(h) $h

      } elseif [regexp {[ \t]*PIN[ \t]+(.+)[ \t]*$} $line lmatch pinname] {
	 # The fill cell is not expected to have any usable pins
	 skip_section $leffile $pinname
      } elseif [regexp {[ \t]*END[ \t]+(.+)[ \t]*$} $line lmatch macrotest] {
         if {"$macrotest" == "$macroname"} {
            break
         } else {
            puts stderr "Unexpected END statement $line while reading macro $macroname"
         }
      }
   }
}

#-----------------------------------------------------------------
# Read the lef macro file and get the fill cells and their widths
#-----------------------------------------------------------------

puts stdout "Reading ${fillcell} macros from LEF file."
flush stdout

set fillcells {}

while {[gets $flef line] >= 0} {
   if [regexp {[ \t]*MACRO[ \t]+(.+)[ \t]*$} $line lmatch macroname] {
      # Parse the "macro" statement
      parse_macro $flef $macroname
      if {[string first $fillcell $macroname] == 0} {
	 # Remember this for later if it's a fill cell
	 lappend fillcells $macroname
      }
   } elseif [regexp {[ \t]*LAYER[ \t]+([^ \t]+)} $line lmatch layername] {
      skip_section $flef $layername
   } elseif [regexp {[ \t]*VIA[ \t]+([^ \t]+)} $line lmatch vianame] {
      skip_section $flef $vianame
   } elseif [regexp {[ \t]*VIARULE[ \t]+([^ \t]+)} $line lmatch viarulename] {
      skip_section $flef $viarulename
   } elseif [regexp {[ \t]*SITE[ \t]+(.+)[ \t]*$} $line lmatch sitename] {
      skip_section $flef $sitename
   } elseif [regexp {[ \t]*UNITS[ \t]*$} $line lmatch] {
      skip_section $flef UNITS
   } elseif [regexp {[ \t]*SPACING[ \t]*$} $line lmatch] {
      skip_section $flef SPACING
   } elseif [regexp {[ \t]*END[ \t]+LIBRARY[ \t]*$} $line lmatch] {
      break
   } elseif [regexp {^[ \t]*#} $line lmatch] {
      # Comment line, ignore.
   } elseif ![regexp {^[ \t]*$} $line lmatch] {
      # Other things we don't care about
      set matches 0
      if [regexp {[ \t]*NAMESCASESENSITIVE} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*VERSION} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*BUSBITCHARS} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*DIVIDERCHAR} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*USEMINSPACING} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*CLEARANCEMEASURE} $line lmatch] {
         incr matches
      } elseif [regexp {[ \t]*MANUFACTURINGGRID} $line lmatch] {
         incr matches
      } else {
         puts stderr "Unexpected input in LEF file:  Only macro defs were expected!"
         puts -nonewline stdout "Line is: $line"
	 flush stdout
      }
   }
}

# If the macro file doesn't define any fill cells, there's not a
# whole lot we can do. . .

if {[llength $fillcells] == 0} {
   puts stdout "No fill cells (${fillname}) found in macro file ${lefname}!"
   exit 1
}

close $flef

# Sort array of fill cells by width

set fillwidths {}
foreach macro $fillcells {
   lappend fillwidths [list $macro [subst \$${macro}(w)]]
}
set fillwidths [lsort -decreasing -index 1 -real $fillwidths]
set fillinfo [lindex $fillwidths 0]
set fillmacro [lindex $fillinfo 0]
set fillvalue [lindex $fillinfo 1]

#------------------------------------------------------------------------
# Now read the contents of the cinfo file so that we have a list of the
# cells to add padding to
#------------------------------------------------------------------------

if {$scale > 0.0} {

   gets $finf line		;# Throw-away line
   gets $finf line		;# Throw-away line
   gets $finf line		;# Failures: X Y

   if {![regexp {[ \t]*Failures:[ \t]+([0-9]+)[ \t]+([0-9]+)} $line \
		lmatch failures numnets]} {
      puts stdout "Cannot parse number of route failures from .cinfo file!"
      exit 1
   }

   gets $finf line		;# Throw-away line

   #------------------------------------------------------------------------
   # Scale and offset adjustments
   #------------------------------------------------------------------------

   set failratio [expr {($failures + 0.0) / $numnets}]

   # Redefine scale and offset so that the calculations are easier
   # Adjust the scale according to the fail ratio

   set scale [expr {1.0 / $scale}]
   set scale [expr {$scale + 60 * $failratio}]
   set offset [expr {$offset - (1.0 / $scale)}]

   #------------------------------------------------------------------------

   set instlist {}
   set filllist {}
   while {[gets $finf line] >= 0} {
      if [regexp {[ \t]*([^ \t]+)[ \t]+([^ \t]+)} $line lmatch instname congest] {
         set numfill [expr {int(($congest - $offset) * $scale + 0.5)}]
         if {$numfill > 0} {
            lappend instlist $instname
            lappend filllist $numfill
         }
      }   
   }

   close $finf
} else {

   #------------------------------------------------------------------------
   # Fixed density planning (No .cinfo file, just fixed density value)
   # Read .cel file to get a list of all instances.  Record the total width
   # of all cells.  Determine how many fill cells need to be added to
   # reduce the density of actively routed cells to the specified amount.
   # Randomly assign fill cells until the density requirement is met.
   #------------------------------------------------------------------------

   set totalwidth 0
   set instlist {}
   set filllist {}
   while {[gets $fcel line] >= 0} {
      if [regexp {[ \t]*cell[ \t]*([0-9]+)[ \t]+([^ \t]+)} $line \
		lmatch instnum instname] {
         lappend instlist $instname
	 gets $fcel line
	 regexp {[ \t]*left[ \t]+([-]*[0-9]+)[ \t]+right[ \t]+([-]*[0-9]+)} $line \
		lmatch left right
	 incr totalwidth [expr {$right - $left}]
      }
   }

   set filltotal [expr {$totalwidth * ((1.0 / $density) - 1.0)}]
   set numfills [expr {int($filltotal / $fillvalue + 0.5)}]
   set numcells [llength $instlist]
   set basefills 0

   # If there are multiple entries in the fill cell list, choose
   # a width such that the number of fill cells and number of
   # standard cells is more or less the same, preferably with
   # more fill cells than standard cells.  Distribute the
   # fill cells among the standard cells.

   if {($numfills < $numcells) && ([llength $fillwidths] > 1)} {
      foreach fillinfo $fillwidths {
	 set fillmacro [lindex $fillinfo 0]
	 set fillvalue [lindex $fillinfo 1]
	 set numfills [expr {int($filltotal / $fillvalue + 0.5)}]
	 set numcells [llength $instlist]
	 if {$numfills > $numcells} {break}
      }
   }

   # Diagnostic information
   puts stdout ""
   puts stdout "decongest.tcl:"
   puts stdout "Fixed density planning, density = $density"
   puts stdout "Number of cells = $numcells, total width = $totalwidth"
   puts stdout "Width of fill = $fillvalue, total fill cells added = $numfills"
   puts stdout ""

   if {$numfills > $numcells} {
      while {$numfills > $numcells} {
         incr basefills
         set numfills [expr {$numfills - $numcells}]
      }
      set numfills [expr {$numcells - $numfills}]
   }
   incr basefills 1
   for {set i 0} {$i < $numfills} {incr i} {
      lappend filllist $basefills
   }
   incr basefills -1
   for {} {$i < $numcells} {incr i} {
      lappend filllist $basefills
   }

   # Rewind the cel file for the next step
   seek $fcel 0 start
}

#------------------------------------------------------------------------
# Now read the contents of the cel file.  When a cell is found that is
# in the "instlist" list of cells, annotate the next line to increase
# the width by an amount equal to the width of a fill cell.
#
# To do:  Use different fill cell widths according to congestion amount;
# or use multiple fill cells (if different widths are not available).
#------------------------------------------------------------------------

while {[gets $fcel line] >= 0} {
   if [regexp {[ \t]*pad[\t]*[0-9]+} $line lmatch] {
      puts $fanno $line	  ;# append the pad line and continue
   } elseif [regexp {[ \t]*cell[ \t]*([0-9]+)[ \t]+([^ \t]+)} \
		$line lmatch instnum instname] {
      set instidx [lsearch $instlist $instname]
      if {$instidx >= 0} {
	 set fillnum [lindex $filllist $instidx]
	 if {$fillnum > 1} {
	    puts $fanno "cell $instnum ${fillmacro}.${fillvalue}X${fillnum}.${instname}"
	    set adjust [expr {$fillnum * $fillvalue}]
	 } elseif {$fillnum == 0} {
	    puts $fanno "cell $instnum ${instname}"
	    set adjust 0
	 } else {
	    puts $fanno "cell $instnum ${fillmacro}.${fillvalue}.${instname}"
	    set adjust $fillvalue
	 }
         gets $fcel line
         if [regexp {[ \t]*left[ \t]+([^ \t]+)[ \t]+right[ \t]+([^ \t]+)[ \t]+(.*)} \
			$line lmatch left right rest] {
	    set totalwidth [expr {-$left + $right + $adjust}]
	    set fleft [expr {-($totalwidth / 2)}]
	    set fright [expr {$fleft + $totalwidth}]
	    puts $fanno "left $fleft right $fright $rest"
         } else {
            puts $fanno $line	;# failed to parse, so ignore it
         }
      } else {
         puts $fanno $line
      }
   } else {
      puts $fanno $line
   }
}

puts stdout "Done!"
