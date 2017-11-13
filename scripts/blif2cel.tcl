#!/usr/bin/tclsh
#
# Usage:
#	blif2cel.tcl <blif_filename> <lef_filename> [<cel_filename>]
#
# If cel_filename is not specified, then name will be the root name
# of the .blif file with the .cel extension.
#
# NOTE:	Cells must be placed relative to the LEF bounding box during
#	physical placement!
#
#------------------------------------------------------------
#   Format translation between:
#	1) LEF library file (input),
#	2) .blif (mapped) netlist file (input), and
#	3) .cel (output) file for GrayWolf placement
#------------------------------------------------------------
#
# Written by Tim Edwards July 25, 2006  MultiGiG, Inc.
# Modified October 8, 2013 to use blif netlists as input 
# Modified December 6, 2016 with hard macro extensions
# courtesy of David Lanzendorfer
#------------------------------------------------------------
# LEF dimensions are microns unless otherwise stated.

set units 100
set pitchx 0		;# value overridden from LEF file
set pitchy 0		;# value overridden from LEF file
set trackskip 0		;# reduce number of implicit feedthroughs
			;# to avoid routing congestion

set arglen [llength $argv]
set index 0
while {$index < $arglen} {
    set arg [lindex $argv $index]
    switch -exact $arg {

	--hard-macro {
	    lappend args(hard_macros) [lindex $argv [incr index]]
	}

	--lef {
	    #lappend args(lefs) [lindex $argv [incr index]]
	    set args(lef) [lindex $argv [incr index]]
	}

	--blif {
	    set args(blif) [lindex $argv [incr index]]
	}

	--cel {
	    set args(cel) [lindex $argv [incr index]]
	}

	--pad-config {
	    set padconfig [lindex $argv [incr index]]
	}

	--pad-width {
	    set padx_len [lindex $argv [incr index]]
	}

	--pad-height {
	    set pady_len [lindex $argv [incr index]]
	}

	--vdd-net {
	    set vddnet [lindex $argv [incr index]]
	}

	--gnd-net {
	    set gndnet [lindex $argv [incr index]]
	}

	--units {
	    set units [lindex $argv [incr index]]
	}

	--force-power-net {
	    set power_force 
	}

	--no-pads {
	    set no_pads true
	}

	default  {
	    set filename [lindex $argv $index]
	}
    }
    incr index
}

if {[info exists args(hard_macros)]} {
    set hardmacros $args(hard_macros)
} else {
    set hardmacros {}
}

if {[info exists args(lef)]} {
    set leffile $args(lef)
} else {
    puts stderr "No LEF file defined"
    exit
}

if {[info exists args(blif)]} {
    set bliffile $args(blif)
    set cellname [file rootname [file tail $bliffile]]
} else {
    puts stderr "No BLIF file defined"
    exit
}

if {[info exists args(cel)]} {
	set celfile $args(cel)
} else {
    puts stderr "No CEL file defined"
    exit
}

if {[info exists padconfig]} {
   if {!([file exists $padconfig])} {
      puts stderr "File $padconfig doesn't exist"
      exit
   }
}

#-------------------------------------------------------------

set lefname [file rootname $leffile]
if {"$lefname" == "$leffile"} {
    set leffile ${lefname}.lef
}

#-------------------------------------------------------------
# Open files for read and write

if [catch {open $leffile r} flef] {
    puts stderr "Error: second argument is not a LEF file ($leffile)!"
    exit 0
} else {
    puts "Opened LEF file ($leffile) for reading..."
}

if [catch {open $bliffile r} fnet] {
    puts stderr "Error: can't open file $bliffile for reading!"
    exit 0
} else {
    puts "Opened BLIF file ($bliffile) for reading ..."
}

if [catch {open $celfile w} fcel] {
    puts stderr "Error: can't open file $celfile for writing!"
    exit 0
} else {
    puts "Opened CEL file ($celfile) for writing..."
}

set fhardmacro {}
foreach hardmacro $hardmacros {
    if [catch {open $hardmacro r} fhardmacro_tmp] {
	puts stderr "Error: can't open file $hardmacro for reading!"
	exit 0
    } else {
	puts "Loaded $hardmacro file for reading..."
	lappend fhardmacro $fhardmacro_tmp
    }
}

#----------------------------------------------------------------
# First, parse the contents of the .blif file and get a list
# of all macro names used.
#----------------------------------------------------------------

puts stdout "1st pass of blif file ${bliffile}. . ."
flush stdout

set macrolist {}
while {[gets $fnet line] >= 0} {
    if [regexp {^[ \t]*\.gate[ \t]+([^ \t]+)} $line lmatch macro] {
	lappend macrolist $macro
    }
}
set macrolist [lsort -unique $macrolist]
close $fnet

#----------------------------------------------------------------
# Parse port information for a macro pin from the LEF MACRO block
#
# Note that all of the geometry of each port gets whittled down
# to a single point.  Maybe GrayWolf can be made to work on
# more complicated port geometry?
#----------------------------------------------------------------

proc parse_port {pinname macroname leffile ox oy} {
    global $macroname units

    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*LAYER[ \t]+(.+)[\t ]*;} $line lmatch layername] {
	    if {![regexp {.*(\d).*} $layername lmatch layernum]} {set layernum 0}
	    set ${macroname}(${pinname},layer) $layernum
	} elseif [regexp {[ \t]*RECT[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)[ \t]+(.+)[ \t]*;} \
			$line lmatch llx lly urx ury] {
	    set llx [expr {int($llx * $units)}]
	    set lly [expr {int($lly * $units)}]
	    set urx [expr {int($urx * $units)}]
	    set ury [expr {int($ury * $units)}]
	    set xp [expr {(($llx + $urx) / 2) - $ox}]
	    set yp [expr {(($lly + $ury) / 2) - $oy}]
	    set ${macroname}(${pinname},xp) $xp
	    set ${macroname}(${pinname},yp) $yp
	} elseif [regexp {[ \t]*POLYGON[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+.*;} \
			$line lmatch llx lly] {
	    set llx [expr {int($llx * $units)}]
	    set lly [expr {int($lly * $units)}]
	    set xp [expr {$llx - $ox}]
	    set yp [expr {$lly - $oy}]
	    set ${macroname}(${pinname},xp) $xp
	    set ${macroname}(${pinname},yp) $yp
	} elseif [regexp {[ \t]*END[ \t]*$} $line lmatch] { break }
    }
    # puts -nonewline stdout "${pinname}"
    # puts -nonewline stdout " [set ${macroname}(${pinname},xp)]"
    # puts -nonewline stdout " [set ${macroname}(${pinname},yp)]"
    # puts -nonewline stdout " [set ${macroname}(${pinname},layer)]"
    # puts stdout ""
}

#----------------------------------------------------------------
# Parse pin information from the LEF MACRO block
#----------------------------------------------------------------

proc parse_pin {pinname macroname leffile ox oy} {
    global $macroname
    # puts "parse_pin"
    # puts "macroname: $macroname, pinname: $pinname"

    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*PORT} $line lmatch] {
	    parse_port $pinname $macroname $leffile $ox $oy
	} elseif [regexp {[ \t]*DIRECTION[ \t]+([^ \t]+)[ \t]*;} $line lmatch porttype] {
	    # puts "Port type: $porttype"
	    set ${macroname}(${pinname},type) [string trim $porttype]
	} elseif [regexp {[ \t]*DIRECTION[ \t]+([^:]+);} $line lmatch porttype] {
	    # puts "Port type: $porttype"
	    set ${macroname}(${pinname},type) [string trim $porttype]
	} elseif [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch pintest] {
	    if {"$pintest" == "$pinname"} {
		break
	    } else {
		puts stdout "Unexpected END statement $line while parsing pin $pinname"
	    }
	}
    }
}

#----------------------------------------------------------------
# Read through a section that we don't care about.
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
# Parse a layer section for routing information
#----------------------------------------------------------------

proc parse_layer {leffile layername} {
    global pitchx pitchy units

    set pitch 0
    set type NONE

    while {[gets $leffile line] >= 0} {
	regexp {[ \t]*TYPE[ \t]+(.+)[ \t]*;} $line lmatch type
	regexp {[ \t]*DIRECTION[ \t]+(.+)[ \t]*;} $line lmatch direc
	regexp {[ \t]*WIDTH[ \t]+(.+)[ \t]*;} $line lmatch width
	regexp {[ \t]*SPACING[ \t]+(.+)[ \t]*;} $line lmatch space
	set p2 [regexp {[ \t]*PITCH[ \t]+(.+)[ \t]+(.+)[ \t]*;} $line \
		lmatch xpitch ypitch]
	set p1 [regexp {[ \t]*PITCH[ \t]+(.+)[ \t]*;} $line lmatch pitch]
	if [regexp {[ \t]*END[ \t]+(.+)[ \t]*$} $line lmatch layertest] {
	    if {"$layertest" != "$layername"} {
		puts -nonewline stderr "Unexpected END statement $line while "
		puts stderr "reading layer $layername"
	    }
	    break
	}
    }

    # All we want to do here is determine the horizontal and vertical
    # route pitches

    if {$p2 == 1} {
	set tpitch [expr int($units * $xpitch)]
	set pitchx $tpitch
	set tpitch [expr int($units * $ypitch)]
	set pitchy $tpitch
    } elseif {$p1 == 1} {
	set tpitch [expr int($units * $pitch)]
	if {$tpitch > 0} {
            if {[string first "HORIZONTAL" $direc] == 0} {
		if {$pitchy == 0} {
		    puts stdout "LEF file specifies route layer Y pitch as $pitch"
		    set pitchy $tpitch
		} elseif {$tpitch < $pitchy} {
		    set pitchy $tpitch
		    puts stdout "LEF file specifies route layer Y pitch as $pitch"
		}
	    } else {
		if {$pitchx == 0} {
		    puts stdout "LEF file specifies route layer X pitch as $pitch"
		    set pitchx $tpitch
		} elseif {$tpitch < $pitchx} {
		    puts stdout "LEF file specifies route layer X pitch as $pitch"
		    set pitchx $tpitch
		}
	    }
	}
    } elseif {$type == "ROUTING"} {
	puts stdout "LEF file route layer ${layername} has no pitch information"
    }
}

#----------------------------------------------------------------
# Parse the hard macro contents of the LEF file and retain the information
# about cell size and pin positions.
#----------------------------------------------------------------

proc parse_hard_macro {leffile macroname} {
    global $macroname units

    # puts stderr "Parsing hard macro $macroname:  Ports are:"
    puts stderr "Parsing hard macro $macroname"
    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*SYMMETRY[ \t]+(.+)[ \t]*;} $line lmatch symmetry] {
	    set ${macroname}(symmetry) $symmetry
	} elseif [regexp {[ \t]*ORIGIN[ \t]+(.+)[ \t]+(.+)[ \t]*;} $line lmatch x y] {
	    set x [expr {round($x * $units)}]
	    set y [expr {round($y * $units)}]
	    set ${macroname}(x) $x
	    set ${macroname}(y) $y
	} elseif [regexp {[ \t]*SIZE[ \t]+(.+)[ \t]+BY[ \t]+(.+)[ \t]*;} \
			$line lmatch w h] {
	    set w [expr {round($w * $units)}]
	    set h [expr {round($h * $units)}]
	    set ${macroname}(w) $w
	    set ${macroname}(h) $h

	    # Compute derived values
	    # ox, oy are the LEF coordinates where GrayWolf expects the "origin"
	    set ox [expr {$x + ($w / 2)}]
	    set oy [expr {$y + ($h / 2)}]
	    set left [expr {-($w / 2)}]
	    set right [expr {$left + $w}]
	    set bottom [expr {-($h / 2)}]
	    set top [expr {$bottom + $h}]
	    set ${macroname}(ox) $ox
	    set ${macroname}(oy) $oy
	    set ${macroname}(left) $left
	    set ${macroname}(right) $right
	    set ${macroname}(top) $top
	    set ${macroname}(bottom) $bottom
	} elseif [regexp {[ \t]*PIN[ \t]+(.+)[ \t]*$} $line lmatch pinname] {
	    # Ignore additional information from LEF version 5.6 syntax
	    set pinname [lindex $pinname 0]
	    parse_pin $pinname $macroname $leffile $ox $oy
	} elseif [regexp {[ \t]*END[ \t]+(^:+)[ \t]*$} $line lmatch macrotest] {
	    if {"$macrotest" == "$macroname"} {
		break
	    } else {
		puts -nonewline stderr "Unexpected END statement $line while "
		puts stderr "reading macro $macroname"
	    }
	}
    }
}


#----------------------------------------------------------------
# Parse the macro contents of the LEF file and retain the information
# about cell size and pin positions.
#----------------------------------------------------------------

proc parse_macro {leffile macroname} {
    global $macroname units

    # puts stderr "Parsing macro $macroname:  Ports are:"
    puts stderr "Parsing macro $macroname"
    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*SYMMETRY[ \t]+(.+)[ \t]*;} $line lmatch symmetry] {
	    set ${macroname}(symmetry) $symmetry
	} elseif [regexp {[ \t]*ORIGIN[ \t]+(.+)[ \t]+(.+)[ \t]*;} $line lmatch x y] {
	    set x [expr {round($x * $units)}]
	    set y [expr {round($y * $units)}]
	    set ${macroname}(x) $x
	    set ${macroname}(y) $y
	} elseif [regexp {[ \t]*SIZE[ \t]+(.+)[ \t]+BY[ \t]+(.+)[ \t]*;} \
			$line lmatch w h] {
	    set w [expr {round($w * $units)}]
	    set h [expr {round($h * $units)}]
	    set ${macroname}(w) $w
	    set ${macroname}(h) $h

	    # Compute derived values
	    # ox, oy are the LEF coordinates where GrayWolf expects the "origin"
	    set ox [expr {$x + ($w / 2)}]
	    set oy [expr {$y + ($h / 2)}]
	    set left [expr {-($w / 2)}]
	    set right [expr {$left + $w}]
	    set bottom [expr {-($h / 2)}]
	    set top [expr {$bottom + $h}]
	    set ${macroname}(ox) $ox
	    set ${macroname}(oy) $oy
	    set ${macroname}(left) $left
	    set ${macroname}(right) $right
	    set ${macroname}(top) $top
	    set ${macroname}(bottom) $bottom
	} elseif [regexp {[ \t]*PIN[ \t]+(.+)[ \t]*$} $line lmatch pinname] {
	    # Ignore additional information from LEF version 5.6 syntax
	    set pinname [lindex $pinname 0]
	    parse_pin $pinname $macroname $leffile $ox $oy
	} elseif [regexp {[ \t]*END[ \t]+(.+)[ \t]*$} $line lmatch macrotest] {
	    if {"$macrotest" == "$macroname"} {
		break
	    } else {
		puts -nonewline stderr "Unexpected END statement $line while "
		puts stderr "reading macro $macroname"
	    }
	}
    }
}

puts stdout "Reading macros from LEF file. . ."
flush stdout

while {[gets $flef line] >= 0} {
    if [regexp {[ \t]*LAYER[ \t]+(.+)[ \t]*$} $line lmatch layername] {
	parse_layer $flef $layername
    } elseif [regexp {[ \t]*MACRO[ \t]+(.+)[ \t]*$} $line lmatch macroname] {
	if {[lsearch $macrolist $macroname] >= 0} {
	    # Parse the "macro" statement 
	    parse_macro $flef $macroname
	} else {
	    # This macro is not used. . . skip to end of macro
	    while {[gets $flef line] >= 0} {
		if [regexp {[ \t]*END[ \t]+(.+)[ \t]*$} $line lmatch macrotest] {
		    if {"$macroname" == "$macrotest"} {
			break
		    }
		}
	    }
	}
    } elseif [regexp {[ \t]*VIA[ \t]+([^ \t]+)} $line lmatch vianame] {
	skip_section $flef $vianame
    } elseif [regexp {[ \t]*VIARULE[ \t]+([^ \t]+)} $line lmatch viarulename] {
	skip_section $flef $viarulename
    } elseif [regexp {[ \t]*NONDEFAULTRULE[ \t]+([^ \t]+)} $line lmatch rulename] {
	skip_section $flef $rulename
    } elseif [regexp {[ \t]*SITE[ \t]+(.+)[ \t]*$} $line lmatch sitename] {
	skip_section $flef $sitename
    } elseif [regexp {[ \t]*UNITS[ \t]*$} $line lmatch] {
	skip_section $flef UNITS
    } elseif [regexp {[ \t]*SPACING[ \t]*$} $line lmatch] {
	skip_section $flef SPACING
    } elseif [regexp {[ \t]*PROPERTYDEFINITIONS[ \t]*$} $line lmatch] {
	skip_section $flef PROPERTYDEFINITIONS
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
	} elseif [regexp {[ \t]*MAXVIASTACK} $line lmatch] {
	    # We *should* care about this one;  need to pass it on to the router
	    incr matches
	} else {
            puts stderr "Unexpected input in LEF file:  Only macro defs were expected!"
            puts stdout "Line is: $line"
	}
    }
}
 
set label_hard_macro {}
foreach fhardmacro $fhardmacro { 
    puts stdout "Reading macros from hard macro LEF file. . ."
    flush stdout

    while {[gets $fhardmacro line] >= 0} {
	if [regexp {[ \t]*LAYER[ \t]+(.+)[ \t]*$} $line lmatch layername] {
	    parse_layer $fhardmacro $layername
	} elseif [regexp {[ \t]*MACRO[ \t]+(.+)[ \t]*$} $line lmatch macroname] {
	    if {[lsearch $macrolist $macroname] >= 0} {
		# Parse the "macro" statement 
		parse_hard_macro $fhardmacro $macroname
		lappend label_hard_macro $macroname
	    } else {
		# This macro is not used. . . skip to end of macro
		while {[gets $fhardmacro line] >= 0} {
		    if [regexp {[ \t]*END[ \t]+(^:+)[ \t]*$} $line lmatch macrotest] {
			if {"$macroname" == "$macrotest"} {
			    break
			}
		    }
		}
	    }
	} elseif [regexp {[ \t]*VIA[ \t]+([^ \t]+)} $line lmatch vianame] {
	    skip_section $fhardmacro $vianame
	} elseif [regexp {[ \t]*VIARULE[ \t]+([^ \t]+)} $line lmatch viarulename] {
	    skip_section $fhardmacro $viarulename
	} elseif [regexp {[ \t]*NONDEFAULTRULE[ \t]+([^ \t]+)} $line lmatch rulename] {
	    skip_section $fhardmacro $rulename
	} elseif [regexp {[ \t]*SITE[ \t]+(.+)[ \t]*$} $line lmatch sitename] {
	    skip_section $fhardmacro $sitename
	} elseif [regexp {[ \t]*UNITS[ \t]*$} $line lmatch] {
	    skip_section $fhardmacro UNITS
	} elseif [regexp {[ \t]*SPACING[ \t]*$} $line lmatch] {
	    skip_section $fhardmacro SPACING
	} elseif [regexp {[ \t]*PROPERTYDEFINITIONS[ \t]*$} $line lmatch] {
	    skip_section $fhardmacro PROPERTYDEFINITIONS
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
	    } elseif [regexp {[ \t]*MAXVIASTACK} $line lmatch] {
	    # We *should* care about this one;  need to pass it on to the router
		incr matches
	    } else {
		puts -nonewline stderr "Unexpected input in LEF file:  Only "
	 	puts stderr "macro defs were expected!"
		puts stdout "Line is: $line"
	    }
	}
    }
}

# If leffile didn't set pitch, then use defaults
if {$pitchx == 0} {
    set pitchx 160
}
if {$pitchy == 0} {
    set pitchy 200
}

#----------------------------------------------------------------
# Parse the contents of the .blif file again and dump each cell
# instance to the .cel file output.

puts stdout "2nd pass of blif file. . ."
flush stdout

set fnet [open $bliffile r]
set mode none
set i 0
set hi 0

while {[gets $fnet line] >= 0} {
    if [regexp {^[ \t]*\.gate[ \t]+([^ \t]+)[ \t]+(.*)$} $line lmatch macroname rest] {
	set mode $macroname
	set left [set ${mode}(left)]
	set right [set ${mode}(right)]
	set width [set ${mode}(w)]
	set top [set ${mode}(top)]
	set bottom [set ${mode}(bottom)]
	if {[catch {incr ${mode}(count)}]} {set ${mode}(count) 0}
	set j [set ${mode}(count)]

	set is_hard_macro False 
	foreach testname $label_hard_macro {
	    if {$testname==$macroname} {
		set is_hard_macro True
	    }
	}

	if $is_hard_macro {
	    puts "$macroname is a hard macro"
	    #puts $fcel "fixed neighborhood 0 from L"
	    #puts $fcel "corners 4 $left $bottom $right $bottom $left $top $right $top"
	    #puts $fcel "class 0 orientations 0 2"
	    puts $fcel "hardcell $i ${mode}_$j"
	    puts $fcel "left $left right $right bottom $bottom top $top"
	} else {
	    puts $fcel "cell $i ${mode}_$j"
	    puts $fcel "left $left right $right bottom $bottom top $top"
	}
	incr i

	# Follow this with implicit feedthroughs, as many as will fit
	# set feedx [expr {$left + ($pitchx / 2)}]
	set feedx [expr {$left + ($pitchx / 2) + ($trackskip * $pitchx)}]
	set k 1
	while {$feedx < $right} {
	    puts $fcel "pin name twfeed${k} signal TW_PASS_THRU layer 1 $feedx $bottom"
	    puts $fcel "   equiv name twfeed${k} layer 1 $feedx $top"
	    set feedx [expr {$feedx + $pitchx}]
	    incr k
	}

	# In the middle of parsing an instance;  mode = instance name (in lowercase).
	foreach pinsig $rest {
	    if [regexp {([^ \t=]+)=([^ \t]+)} $pinsig lmatch pinname netname] {
		regsub -all "<" $pinname "\[" pinname
		regsub -all ">" $pinname "\]" pinname
		set pinx [set ${mode}(${pinname},xp)]
		set piny [set ${mode}(${pinname},yp)]
		set pinlayer [set ${mode}(${pinname},layer)]
		set pintype [set ${mode}(${pinname},type)]

		# Is signal a buffer tree?  If so, make it swappable, if it's an input
		if [regexp {(.*)_bF\$buf[0-9]+$} $netname lmatch sigpfix] {
		    if  {$pintype == "INPUT"} {
			puts $fcel "pin_group"
			puts -nonewline $fcel "pin name ${sigpfix}_bF\$pin/$pinname "
			puts $fcel "signal $netname layer $pinlayer $pinx $piny"
			puts $fcel "end_pin_group"
		    } else {
			puts -nonewline $fcel "pin name $pinname signal $netname "
			puts $fcel "layer $pinlayer $pinx $piny"
		    }
		} else {
		    puts -nonewline $fcel "pin name $pinname signal $netname layer "
		    puts $fcel "$pinlayer $pinx $piny"
		}
	    }
	    flush stdout
	}

	if {[info exists power_force]} {
		if {[info exists vddnet]} {
			set pinx [set ${mode}(${vddnet},xp)]
			set piny [set ${mode}(${vddnet},yp)]
			set pinlayer [set ${mode}(${vddnet},layer)]
			set pintype [set ${mode}(${vddnet},type)]
			puts -nonewline $fcel "pin name $vddnet signal $vddnet layer "
			puts $fcel "$pinlayer $pinx $piny"
		}

		if {[info exists gndnet]} {
			set pinx [set ${mode}(${gndnet},xp)]
			set piny [set ${mode}(${gndnet},yp)]
			set pinlayer [set ${mode}(${gndnet},layer)]
			set pintype [set ${mode}(${gndnet},type)]
			puts -nonewline $fcel "pin name $gndnet signal $gndnet layer "
			puts $fcel "$pinlayer $pinx $piny"
		}
	}


    } elseif [regexp {^[ \t]*\.inputs} $line lmatch] {
	set mode "pins"
    } elseif [regexp {^[ \t]*\.outputs} $line lmatch] {
	set mode "pins"
    } elseif [regexp {^[ \t]*\.model[ \t]+([^ \t]+)} $line lmatch cellverify] {
	if {"$cellname" != "$cellverify"} {
	    puts -nonewline stderr "WARNING:  model name ${cellverify} does not"
	    puts stderr " match filename ${cellname}!"
	}
    }
}
close $fnet

#----------------------------------------------------------------
# Parse the contents of the .blif file again and dump each input or output
# to the .cel file as a "pad".

puts stdout "3rd pass of blif file. . ."
flush stdout

set pmul 1

set pad_fixed False
array set pad_positions {}
set placedpads {}
set powerpads {}

if {[info exists padconfig]} {
	set pad_fixed True
	set fpads [open $padconfig r]
	while {[gets $fpads line] >= 0} {
		if [regexp {^[ \t]*px:[ \t]+(.*)$} $line lmatch len] {
			set px $len
		}
		if [regexp {^[ \t]*py:[ \t]+(.*)$} $line lmatch len] {
			set py $len
		}
		if [regexp {^[ \t]*side[ \t]+(.*):[ \t]+(.*)$} $line lmatch side bank] {
			foreach fixed_pad $bank {
				set pad_positions($fixed_pad) $side
			}
		}
		if [regexp {^[ \t]*power pads:[ \t]+(.*)$} $line lmatch ppads] {
			foreach ppad $ppads {
				lappend powerpads $ppad
			}
		}
		if [regexp {^[ \t]*power multiplier:[ \t]+(.*)$} $line lmatch pfactor] {
			set pmul $pfactor
		}
		if [regexp {^[ \t]*side space:[ \t]+(.*)$} $line lmatch len] {
			set side_space $len
		}
	}
}

if {[info exists padx_len]} {
	set px $padx_len
} else {
	set px [expr int($pitchx / 2)]
}

if {[info exists pady_len]} {
	set py $pady_len
} else {
	set py [expr int($pitchy / 2)]
}

set fnet [open $bliffile r]
set mode none
set padnum 1
while {[gets $fnet line] >= 0} {
    if [regexp {^[ \t]*\.inputs[ \t]+(.*)$} $line lmatch rest] {
	set mode inputs
	set line $rest
    } elseif [regexp {^[ \t]*\.outputs[ \t]+(.*)$} $line lmatch rest] {
	set mode outputs
	set line $rest
    } elseif [regexp {^[\ t]*\.gate} $line lmatch] {
	break;
    } elseif [regexp {^[\ t]*\.end} $line lmatch] {
	break;
    }

    if [regexp {^(.*)[\\]+$} $line lmatch rest] {
	set line $rest
    }

    if {[info exists no_pads]} {
	puts stdout "Not adding pads, because we will get a static pad frame mapping"
    } else {
	if {$mode == "inputs" || $mode == "outputs"} {
	    foreach pinname $line {
		puts $fcel "pad $padnum name twpin_$pinname"
		puts $fcel "corners 4 -$px -$py -$px $py $px $py $px -$py"
		puts $fcel "pin name $pinname signal $pinname layer 1 0 0"
		puts $fcel ""
		incr padnum
	    }
	}
    }
}

foreach pinname $powerpads {
	for {set i 1} {$i <= $pmul} {incr i} {
		foreach side [list "T" "L" "R" "B"] {
			puts $fcel "pad $padnum name twpin_$side.$pinname<$i>"
			puts $fcel "corners 4 -$px -$py -$px $py $px $py $px -$py  restrict side $side"
			puts $fcel "pin name $pinname signal $pinname layer 1 0 0"
			puts $fcel ""
			incr padnum
		}
	}
}

puts stdout "Done!"
