#!/usr/bin/tclsh
#---------------------------------------------------------------------------
# addspacers.tcl ---
#
# Read LEF file and parse for fill cells;  get the number and width of
#	the different fill cells available.
# Read the DEF file once, to find the number of rows and the endpoint
#	of each row.  This is not a general purpose script. . . we assume
#	output is from GrayWolf and place2def, so cells are aligned on
#	the left, and components appear in order, row by row, from left
#	to right.
# Read the DEF file again, up to the COMPONENTS section.
# Modify the COMPONENTS section to add the spacer cells, and write out
#	the annotated DEF file.
#
# Updated 6/7/2017 to add option to split layout at intervals and
# insert a stripe of minimum size for the addition of a power bus.
# This is an ad hoc solution that does not consider how the overall
# timing is affected by stretching routes that have to cross this
# stripe.  Use of "-nostretch" will place the stripes over the existing
# layout without making extra space with fill.  This is generally
# preferred unless it makes the cell unroutable.
#---------------------------------------------------------------------------

namespace path {::tcl::mathop ::tcl::mathfunc}

if {$argc < 3} {
    puts stdout "Usage:  addspacers [<options>] <project_name> <lef_file> <fill_cell>"
    puts stdout "Options:"
    puts stdout "  -stripe <width> <pitch> <pattern> [-nostretch] [-techlef <tech.lef>]"
    exit 0
}

puts stdout "Running addspacers.tcl"

# NOTE:  There is no scaling.  GrayWolf values are in centimicrons,
# as are DEF values (UNITS DISTANCE MICRONS 100)

set argidx 0
set dostripes false	;# no power bus striping by default
set stripewidth 0
set stripepitch 0
set stripeoffset 0
set numstripes 0
set dostretch true
set techlef ""
set pitchx 0
set layers {}
set vias {}

while true {
    set arg0 [lindex $argv $argidx]
    if {$arg0 == "-stripe"} {
	incr argidx
	set dostripes true
	set stripewidth [lindex $argv $argidx]
	set stripewidthreq $stripewidth
	incr argidx
	set stripepitch [lindex $argv $argidx]
	incr argidx
	set stripepattern [lindex $argv $argidx]
    } elseif {$arg0 == "-stripewidth"} {
	incr argidx
	set dostripes true
	set stripewidthreq $stripewidth
	set stripewidth [lindex $argv $argidx]
    } elseif {$arg0 == "-stripepitch"} {
	incr argidx
	set dostripes true
	set stripepitch [lindex $argv $argidx]
    } elseif {$arg0 == "-stripepattern"} {
	incr argidx
	set dostripes true
	set stripepattern [lindex $argv $argidx]
    } elseif {$arg0 == "-techlef"} {
	incr argidx
	set techlef [lindex $argv $argidx]
    } elseif {$arg0 == "-nostretch"} {
	set dostretch false
    } else {
	break
    }
    incr argidx
}

set topname [file rootname [lindex $argv $argidx]]
incr argidx
set lefname [lindex $argv $argidx]
incr argidx
set fillcell [lindex $argv $argidx]

set defname ${topname}.def
set defoutname ${topname}_filled.def

set units 100.0		;# write centimicron units into the DEF file

# Input arguments are assumed to be in microns.  Convert them to the
# DEF file units.

set stripewidth [* $units $stripewidth]
set stripepitch [* $units $stripepitch]

set diexlow 0
set diexhigh 0
set dieylow 0
set dieyhigh 0

#-----------------------------------------------------------------
# Open all files for reading and writing
#-----------------------------------------------------------------

if [catch {open $lefname r} flef] {
    puts stderr "Error: can't open LEF file $lefname for input"
    return
}

if [catch {open $defname r} fdef] {
    puts stderr "Error: can't open DEF file $defname for input"
    return
}

if {$techlef != ""} {
    if [catch {open $techlef r} ftech] {
	puts stderr "Warning: can't open LEF tech file $techlef for input"
	puts stderr "Will add obstruction layers but no power/ground stripes"
	set techlef ""
    }
}

#----------------------------------------------------------------
# Parse a via section for routing information
#
# (Note:  this routine only saves one RECT entry for each layer.
# The intention is only to capture the name of the via, the
# overall dimension, and the layers used.)
#----------------------------------------------------------------

proc parse_via {leffile vianame} {
    global units

    set viarec [dict create]
    set type NONE
    dict set viarec name $vianame

    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*LAYER[ \t]+([^ \t]+)[ \t]*;} $line lmatch layername] {
	    continue
	} elseif [regexp {[ \t]*RECT[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]*;} $line lmatch llx lly urx ury] {
	    set llx [* $llx $units]
	    set lly [* $lly $units]
	    set urx [* $urx $units]
	    set ury [* $ury $units]
	    dict set viarec ${layername} [list $llx $lly $urx $ury]
	} elseif [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch viatest] {
	    if {"$viatest" != "$vianame"} {
	        puts -nonewline stderr "Unexpected END statement $line while "
	        puts stderr "reading via $vianame"
	    }
	    break
	}
    }
    return $viarec
}

#----------------------------------------------------------------
# Parse a layer section for routing information
#----------------------------------------------------------------

proc parse_layer {leffile layername} {
    global units

    set layerrec [dict create]
    set pitch 0
    set type NONE
    dict set layerrec name $layername

    if [regexp {[^0-9]*([0-9]+)} $layername lmatch lnum] {
	dict set layerrec number $lnum
    } else {
	dict set layerrec number 0
    }

    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*TYPE[ \t]+([^ \t]+)[ \t]*;} $line lmatch type] {
	    dict set layerrec type $type
	} elseif [regexp {[ \t]*DIRECTION[ \t]+([^ \t]+)[ \t]*;} $line lmatch direc] {
	    dict set layerrec direction $direc
	} elseif [regexp {[ \t]*WIDTH[ \t]+([^ \t]+)[ \t]*;} $line lmatch width] {
	    dict set layerrec width $width
	} elseif [regexp {[ \t]*SPACING[ \t]+([^ \t]+)[ \t]*;} $line lmatch space] {
	    dict set layerrec spacing $space
	} elseif [regexp {[ \t]*PITCH[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]*;} $line \
		lmatch xpitch ypitch] {
	    dict set layerrec xpitch [* $xpitch $units]
	    dict set layerrec ypitch [* $ypitch $units]
	} elseif [regexp {[ \t]*PITCH[ \t]+([^ \t]+)[ \t]*;} $line lmatch pitch] {
	    set pitch [* $pitch $units]
	    continue
	} elseif [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch layertest] {
	    if {"$layertest" != "$layername"} {
	        puts -nonewline stderr "Unexpected END statement $line while "
	        puts stderr "reading layer $layername"
	    }
	    break
	}
    }

    if {$pitch != 0} {
	set orient [dict get $layerrec direction]
	if {$orient == "VERTICAL"} {
	    dict set layerrec xpitch $pitch
	    dict set layerrec ypitch 0
	} elseif {$orient == "HORIZONTAL"} {
	    dict set layerrec ypitch $pitch
	    dict set layerrec xpitch 0
	}
    }
    return $layerrec
}

#----------------------------------------------------------------
# Parse port information for a macro pin from the LEF MACRO block
# Here, used only for power and ground pins.
# Assume that the power and ground ports define one rectangle that
# reaches from end to end of the cell, and record that rectangle.
#----------------------------------------------------------------

proc parse_port {pinname macroname leffile ox oy} {
    global $macroname units

    set x2 [+ $ox [set ${macroname}(w)]]

    while {[gets $leffile line] >= 0} {
        if [regexp {[ \t]*LAYER[ \t]+([^ \t]+)[\t ]*;} $line lmatch layername] {
            if {![regexp {.*(\d).*} $layername lmatch layernum]} {set layernum 0}
            set ${macroname}(${pinname},layer) $layernum
        } elseif [regexp {[ \t]*RECT[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]*;} \
                        $line lmatch llx lly urx ury] {
	    set llx [* $llx $units]
	    set lly [* $lly $units]
	    set urx [* $urx $units]
	    set ury [* $ury $units]
	    if {$llx < $ox} {set llx $ox}
	    if {$urx > $x2} {set urx $x2}
	    if {($llx == $ox) && ($urx == $x2)} {
		set ${macroname}(${pinname},llx) $llx
		set ${macroname}(${pinname},lly) $lly
		set ${macroname}(${pinname},urx) $urx
		set ${macroname}(${pinname},ury) $ury
	    }
	    # To do:  If bus is made of more than one rectangle, merge
	    # leftmost and rightmost rectangles into one entry.

	# NOTE: To do: parse POLYGON records, if this ever comes up in practice.

        } elseif [regexp {[ \t]*END[ \t]*$} $line lmatch] { break }
    }
}

#----------------------------------------------------------------
# Parse pin information from the LEF MACRO block
#----------------------------------------------------------------

proc parse_pin {pinname macroname leffile ox oy} {
    global $macroname

    set portuse ""
    while {[gets $leffile line] >= 0} {
        if [regexp {[ \t]*PORT} $line lmatch] {
	    if {($portuse == "POWER") || ($portuse == "GROUND") || ($portuse == "")} {
	        parse_port $pinname $macroname $leffile $ox $oy
	    }
        } elseif [regexp {[ \t]*DIRECTION[ \t]+([^ \t]+)[ \t]*;} $line lmatch porttype] {
            set ${macroname}(${pinname},type) $porttype
        } elseif [regexp {[ \t]*DIRECTION[ \t]+([^:]+);} $line lmatch porttype] {
            set ${macroname}(${pinname},type) $porttype
        } elseif [regexp {[ \t]*USE[ \t]+([^ \t]+)[ \t]*;} $line lmatch portuse] {
            set ${macroname}(${pinname},use) $portuse
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
# Read through a LEF file section that we don't care about.
#----------------------------------------------------------------

proc skip_section {leffile sectionname} {
    while {[gets $leffile line] >= 0} {
	if [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch sectiontest] {
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
	if [regexp {[ \t]*SYMMETRY[ \t]+([^ \t]+)[ \t]*;} $line lmatch symmetry] {
	    set ${macroname}(symmetry) $symmetry
	} elseif [regexp {[ \t]*ORIGIN[ \t]+(.+)[ \t]+(.+)[ \t]*;} $line lmatch x y] {
	    set x [expr {int($x * $units)}]
	    set y [expr {int($y * $units)}]
	    set ${macroname}(x) $x
	    set ${macroname}(y) $y
	} elseif [regexp {[ \t]*SIZE[ \t]+(.+)[ \t]+BY[ \t]+(.+)[ \t]*;} \
                        $line lmatch w h] {
	    set w [expr {int($w * $units)}]
	    set h [expr {int($h * $units)}]
	    set ${macroname}(w) $w
	    set ${macroname}(h) $h

	} elseif [regexp {[ \t]*PIN[ \t]+([^ \t]+)[ \t]*$} $line lmatch pinname] {
	    # Get power and ground information, the values in the fill cell
	    # will be used to generate power and ground stripes
	    parse_pin $pinname $macroname $leffile $x $y
	} elseif [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch macrotest] {
	    if {"$macrotest" == "$macroname"} {
		break
	    } else {
		puts stderr "Unexpected END statement $line while reading macro $macroname"
	    }
	}
    }
}

#-----------------------------------------------------------------
# Read a LEF file.  Returns a list of fill cells if the LEF file
# was a standard cell set macro file.
#-----------------------------------------------------------------

proc read_lef {flef fillcell} {
    global layers vias
    set fillcells {}
    while {[gets $flef line] >= 0} {
        # Ignore comment lines
        if [regexp {[ \t]*#} $line lmatch] {
	    continue
	} elseif [regexp {[ \t]*MACRO[ \t]+([^ \t]+)[ \t]*$} $line lmatch macroname] {
	    # Parse the "macro" statement
	    parse_macro $flef $macroname
	    if {[string first $fillcell $macroname] == 0} {
		# Remember this for later if it's a fill cell
		lappend fillcells $macroname
	    }
	} elseif [regexp {[ \t]*LAYER[ \t]+([^ \t]+)} $line lmatch layername] {
	    lappend layers [parse_layer $flef $layername]
	} elseif [regexp {[ \t]*VIA[ \t]+([^ \t]+)} $line lmatch vianame] {
	    lappend vias [parse_via $flef $vianame]
	} elseif [regexp {[ \t]*VIARULE[ \t]+([^ \t]+)} $line lmatch viarulename] {
	    skip_section $flef $viarulename
	} elseif [regexp {[ \t]*SITE[ \t]+([^ \t]+)[ \t]*$} $line lmatch sitename] {
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
	    } elseif {$fillcell != ""} {
		puts stderr "Unexpected input in LEF file:  Only macro defs were expected!"
		puts -nonewline stdout "Line is: $line"
		flush stdout
	    }
	}
    }
    return $fillcells
}

#-----------------------------------------------------------------
# Read the tech LEF file for via and layer information
#-----------------------------------------------------------------

if {$techlef != ""} {
    puts stdout "Reading technology LEF file ${techlef}."
    read_lef $ftech ""
}

#-----------------------------------------------------------------
# Read the LEF macro file and get the fill cells and their widths
#-----------------------------------------------------------------

puts stdout "Reading ${fillcell} macros from LEF file."
flush stdout

set fillcells [read_lef $flef $fillcell]

# If the macro file doesn't define any fill cells, there's not a
# whole lot we can do, unless we're adding power stripes without
# stretching.

if {[llength $fillcells] == 0} {
    puts stdout "No fill cells (${fillname}) found in macro file ${lefname}!"
    if {$dostripes == false || $dostretch == true} {exit 1}
}

# Get routing grid X pitch from the layer information

set testpitch 0
foreach layer $layers {
    if {[dict get $layer type] != "ROUTING"} {continue}
    set layerpitch [dict get $layer xpitch]
    if {$layerpitch > 0} {
	if {$testpitch == 0} {
	    set testpitch $layerpitch
	} elseif {$layerpitch < $testpitch} {
	    set testpitch $layerpitch
	}
    }
}
set pitchx $testpitch

# If neither LEF file defined layers, then set "needspecial" to false,
# because there is not enough information to know how to write the
# specialnets section.  Flag a warning.

if {$pitchx == 0} {
    puts stderr "No technology layer information in LEF files."
    puts stderr "Cannot create physical geometry for power and ground."
    set needspecial false
}

#-----------------------------------------------------------------
# Parse the COMPONENTS section of the DEF file
# Assuming this file was generated by place2def, each component
# should be on a single line.
#-----------------------------------------------------------------

proc parse_components {deffile rows} {
    upvar $rows rdict
    while {[gets $deffile line] >= 0} {
        if [regexp {[ \t]*#} $line lmatch] {
	    continue
	} elseif [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch sectiontest] {
	    if {"$sectiontest" != "COMPONENTS"} {
		puts -nonewline stderr "Unexpected END statement $line "
		puts stderr "while reading section COMPONENTS"
	    }
	    break
	} elseif [regexp {[ \t]*-[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+\+[ \t]+PLACED[ \t]+\([ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+\)[ \t]+([^ \t]+)[ \t]+;} $line lmatch \
		instance macro px py orient] {
	    if [catch {set row [dict get $rdict $py]}] {
		dict set rdict $py [list $px $instance $macro $orient]
	    } else {
		set rowmax [lindex $row 0]
		if {$px > $rowmax} {
		    dict set rdict $py [list $px $instance $macro $orient]
		}
	    }
	} else {
	    puts -nonewline stderr "Unexpected statement $line "
	    puts stderr "while reading section COMPONENTS"
	}
    }
}

#-----------------------------------------------------------------
# Get the power pin name from a macro
#-----------------------------------------------------------------

proc get_power {macroname} {
    global $macroname
    foreach name [array names $macroname] {
	if {[set ${macroname}($name)] == "POWER"} {
            set cidx [- [string first "," $name] 1]
 	    return [string range $name 0 $cidx]
        }
    }
    return ""
}

#-----------------------------------------------------------------
# Get the ground pin name from a macro
#-----------------------------------------------------------------

proc get_ground {macroname} {
    global $macroname
    foreach name [array names $macroname] {
	if {[set ${macroname}($name)] == "GROUND"} {
            set cidx [- [string first "," $name] 1]
 	    return [string range $name 0 $cidx]
        }
    }
    return ""
}

#-----------------------------------------------------------------
# Write entries for SPECIALNETS corresponding to the power and
# ground stripes
#-----------------------------------------------------------------

proc write_special {stripeinfo stripefills stripewidth stripepattern
		rows dieylow dieyhigh fanno} {
    global fmin vias layers pitchx

    # Take 1st fill cell and find name of power and ground buses
    set fillmacro [lindex $stripefills 0]
    global $fillmacro

    # Parse layers for topmost vertical route layer.  Unfortunately,
    # LEF syntax is too stupid to define which layer is which, so
    # we rely on common convention of numbering the metal layers.
    set tnum 0
    set topmet 0
    foreach layer $layers {
	if {[dict get $layer type] != "ROUTING"} {continue}
	if {[dict get $layer direction] == "VERTICAL"} {
	    set lnum [dict get $layer number]
	    if {$lnum > $tnum} {
		set tnum $lnum
	    }
	}
	set lnum [dict get $layer number]
	if {$lnum > $topmet} {
	    set topmet $lnum
	}
    }
    if {$tnum < 3} {
	puts stderr "Error:  No vertical routing layer at metal 3 or above"
	puts stderr "Only posts will be placed, and not connected"
    }
 
    # Create list of route layers in order.
    set layerlist {}
    for {set i 1} {$i <= $topmet} {incr i} {
	foreach layer $layers {
	    if {[dict get $layer type] == "ROUTING"} {
		set lnum [dict get $layer number]
		if {$lnum == $i} {
		    lappend layerlist [dict get $layer name]
		}
	    }
	}
    }

    # Parse vias for the smallest size via on each metal layer (except top)
    # TO-DO:  Read VIAGEN entries and generate a via entry the width of the
    # power stripe.

    set basevias {}
    set viawidths {}
    set nvias {}
    set blayer ""
    set tlayer ""
    for {set i 1} {$i <= $topmet} {incr i} {
	foreach layer $layers {
	    if {[dict get $layer type] != "ROUTING"} {continue}
	    set lnum [dict get $layer number]
	    if {$lnum == $i} {
		set blayer [dict get $layer name]
	    } elseif {$lnum == [+ $i 1]} {
		set tlayer [dict get $layer name]
	    }
	}
	set bestvia ""
	set bestarea 0
	foreach via $vias {
	    set areab 0
	    set areat 0
	    foreach vlayer [dict keys $via] {
		if {$vlayer == $blayer} {
		    set coords [dict get $via $vlayer]
		    set llx [lindex $coords 0]
		    set lly [lindex $coords 1]
		    set urx [lindex $coords 2]
		    set ury [lindex $coords 3]
		    set widthb [expr ($urx - $llx)]
		    set areab [expr $widthb * ($ury - $lly)]
		}
		if {$vlayer == $tlayer} {
		    set coords [dict get $via $vlayer]
		    set llx [lindex $coords 0]
		    set lly [lindex $coords 1]
		    set urx [lindex $coords 2]
		    set ury [lindex $coords 3]
		    set widtht [expr ($urx - $llx)]
		    set areat [expr $widtht * ($ury - $lly)]
		}
	    }
	    if {($areab > 0) && ($areat > 0)} {
		set area [max $areab $areat]
		set width [max $widthb $widtht]
		if {($bestvia == "") || ($area < $bestarea)} {
		    set bestvia [dict get $via name]
		    set bestarea $area
		    set bestwidth $width
		}
	    }
	}
	lappend basevias $bestvia
	lappend viawidths $bestwidth
	lappend nvias [expr ($stripewidth / $bestwidth) - 1]

    }
    # topmet will be used to index basevias and layerlist, which are
    # 0 to tnum - 1, not 1 to tnum.
    if {$tnum >= 3} {set topmet [- $tnum 1]}

    set powername [get_power $fillmacro]

    set pllx [set ${fillmacro}(${powername},llx)]
    set plly [set ${fillmacro}(${powername},lly)]
    set purx [set ${fillmacro}(${powername},urx)]
    set pury [set ${fillmacro}(${powername},ury)]
    set h [set ${fillmacro}(h)]
    set y [/ [+ $plly $pury] 2.0]

    puts $fanno "- ${powername}"
    set j 0
    set first true
    foreach stripe $stripeinfo {
	set smean [lindex $stripe 0]
	set slow  [lindex $stripe 1]
	set shigh [lindex $stripe 2]

	if {[- $shigh $slow] > 0} {
	   set smean [/ [+ $shigh $slow] 2.0]
	}
	# Make sure smean is on pitch
	set nw [expr int($smean / $fmin)]
	set smean [* $nw $fmin]

	set slow [- $smean [/ $stripewidth 2.0]]
	set shigh [+ $smean [/ $stripewidth 2.0]]

	set pattern [string index $stripepattern $j]
	if {$pattern == "P"} {
	    set drows [dict keys $rows]
	    foreach rowy $drows {
		set orient [lindex [dict get $rows $rowy] 3]
		if {$orient == "S" || $orient == "FS"} {
		    set ay [- $h $y]
		} else {
		    set ay $y
		}
		for {set i 0} {$i < $topmet} {incr i} {
		    set ry [+ $rowy $ay]
		    set layer [lindex $layerlist $i]
		    set via [lindex $basevias $i]
		    set vw [lindex $viawidths $i]
		    set nv [lindex $nvias $i]
		    set x1 $slow
		    if $first {
		        puts -nonewline $fanno "+ ROUTED "
		        set first false
		    } else {
		        puts -nonewline $fanno "  NEW "
		    }
		    puts $fanno "$layer $vw ( $shigh $ry ) ( $x1 * )"
		    for {set k 0} {$k < $nv} {incr k} {
			set x2 [+ $x1 $vw]
		        puts $fanno "  NEW $layer $vw ( $x1 $ry ) ( $x2 * ) $via"
			set x1 $x2
		    }
		}
	    }
	    # At the end, put top vertical metal stripe from top to bottom
	    if {$tnum >= 3} {
		set layer [lindex $layerlist [- $tnum 1]]
		puts $fanno "  NEW $layer $stripewidth ( $smean $dieylow ) ( * $dieyhigh )"
	    }
	}
	incr j
    }
    puts $fanno " ;"

    set groundname [get_ground $fillmacro]

    set gllx [set ${fillmacro}(${groundname},llx)]
    set glly [set ${fillmacro}(${groundname},lly)]
    set gurx [set ${fillmacro}(${groundname},urx)]
    set gury [set ${fillmacro}(${groundname},ury)]
    set w [- $gury $glly]
    set y [/ [+ $glly $gury] 2.0]

    puts $fanno "- ${groundname}"
    set j 0
    set first true
    foreach stripe $stripeinfo {
	set smean [lindex $stripe 0]
	set slow  [lindex $stripe 1]
	set shigh [lindex $stripe 2]

	if {[- $shigh $slow] > 0} {
	   set smean [/ [+ $shigh $slow] 2.0]
	}
	# Make sure smean is on pitch
	set nw [expr int($smean / $fmin)]
	set smean [* $nw $fmin]

	set slow [- $smean [/ $stripewidth 2.0]]
	set shigh [+ $smean [/ $stripewidth 2.0]]

	set pattern [string index $stripepattern $j]
	if {$pattern == "G"} {
	    set drows [dict keys $rows]
	    foreach rowy $drows {
		set orient [lindex [dict get $rows $rowy] 3]
		if {$orient == "S" || $orient == "FS"} {
		    set ay [- $h $y]
		} else {
		    set ay $y
		}
		for {set i 0} {$i < $topmet} {incr i} {
		    set ry [+ $rowy $ay]
		    set layer [lindex $layerlist $i]
		    set via [lindex $basevias $i]
		    set vw [lindex $viawidths $i]
		    set nv [lindex $nvias $i]
		    set x1 $slow
		    if $first {
		    	puts -nonewline $fanno "+ ROUTED "
			set first false
		    } else {
			puts -nonewline $fanno "  NEW "
		    }
		    puts $fanno "$layer $vw ( $shigh $ry ) ( $x1 * )"
		    for {set k 0} {$k < $nv} {incr k} {
			set x2 [+ $x1 $vw]
			puts $fanno "  NEW $layer $vw ( $x1 $ry ) ( $x2 * ) $via"
			set x1 $x2
		    }
		}
	    }
	    # At the end, put top vertical metal stripe from top to bottom
	    if {$tnum >= 3} {
		set layer [lindex $layerlist [- $tnum 1]]
		puts $fanno "  NEW $layer $stripewidth ( $smean $dieylow ) ( * $dieyhigh )"
	    }
	}
	incr j
    }
    puts $fanno " ;"
}

#-----------------------------------------------------------------
# Read the DEF file once to get the number of rows and the length
# of each row
#-----------------------------------------------------------------

puts stdout "Reading DEF file ${defname}. . ."
flush stdout

while {[gets $fdef line] >= 0} {
    if [regexp {[ \t]*#} $line lmatch] {
	continue
    } elseif [regexp {[ \t]*COMPONENTS[ \t]+([^ \t]+)[ \t]*;} $line lmatch number] {
	set rows [dict create]
	# Parse the "COMPONENTS" statement
	parse_components $fdef rows
    } elseif [regexp {[ \t]*NETS[ \t]+([^ \t]+)} $line lmatch netnums] {
	skip_section $fdef NETS
    } elseif [regexp {[ \t]*SPECIALNETS[ \t]+([^ \t]+)} $line lmatch netnums] {
	skip_section $fdef SPECIALNETS
    } elseif [regexp {[ \t]*PINS[ \t]+([^ \t]+)} $line lmatch pinnum] {
	skip_section $fdef PINS
    } elseif [regexp {[ \t]*VIARULE[ \t]+([^ \t]+)} $line lmatch viarulename] {
	skip_section $fdef $viarulename
    } elseif [regexp {[ \t]*VIA[ \t]+([^ \t]+)[ \t]*$} $line lmatch vianame] {
	lappend vias [parse_via $fdef $vianame]
    } elseif [regexp {[ \t]*END[ \t]+DESIGN[ \t]*$} $line lmatch] {
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
	} elseif [regexp {[ \t]*UNITS} $line lmatch] {
	    incr matches
	} elseif [regexp {[ \t]*DESIGN} $line lmatch] {
	    incr matches
	} elseif [regexp {[ \t]*DIEAREA} $line lmatch] {
	    incr matches
	} elseif [regexp {[ \t]*TRACKS} $line lmatch] {
	    incr matches
	} else {
	    puts stderr "Unexpected input in DEF file:"
	    puts stdout "Line is: $line"
	}
    }
}

close $flef
close $fdef

# Sort array of fill cells by width

set fillwidths {}
foreach macro $fillcells {
    lappend fillwidths [list $macro [set ${macro}(w)]]
}
set fillwidths [lsort -decreasing -index 1 $fillwidths]

# For each row, add the width of the last cell in that row
# to get the row end X value

dict for {row rowvals} $rows {
    set xmax [lindex $rowvals 0]
    set macro [lindex $rowvals 2]
    set xmax [+ $xmax [set ${macro}(w)]]
    set rowvals [lreplace $rowvals 0 0 $xmax]
    dict set rows $row $rowvals
}
   
# Find number of rows and longest row
set numrows 0
set rowmax 0
dict for {row rowvals} $rows {
    incr numrows
    set xmax [lindex $rowvals 0]
    if {$xmax > $rowmax} {set rowmax $xmax}
}
puts stdout "Number of rows is $numrows"
puts stdout "Longest row has width [/ $rowmax $units] um"

# Now, for each row, find the difference between the row end and row max,
# and create a list of how many of each fill macro it takes to fill the
# row out to the maximum distance

set numfills 0
dict for {row rowvals} $rows {
    set xmax [lindex $rowvals 0]
    set xd [- $rowmax $xmax]
    set fills {}
    foreach fillset $fillwidths {
	set fw [lindex $fillset 1]
	set fn [floor [/ $xd $fw]]
	lappend fills [list [lindex $fillset 0] [int $fn]]
	set xd [- $xd [* $fn $fw]]
	incr numfills [int $fn]
    }
    lappend rowvals $fills
    dict set rows $row $rowvals
}
set numcomps [+ $number $numfills]

# Analyze design for power striping

if {$dostripes} {
    set stripefills {}
}

if {$dostripes && ($rowmax > $stripepitch)} {

    # What sizes of fill make up the stripe width, to the nearest unit cell?

    set xtot 0
    while {1} {
	set fidx 0
	set fmax 0
	set fmin -1
	set i 0
	foreach fillset $fillwidths {
	    set fw [lindex $fillset 1]
	    set ftest [+ $fw $xtot]
	    if {($ftest <= $stripewidth) && ($ftest > $fmax)} {
		set fmax $fw
		set fidx $i
	    }
	    if {$fmin < 0} {
		set fmin $fw
	    } elseif {$fmin > $fw} {
		set fmin $fw
	    }
	    incr i
	}
	if {$fmax == 0} {
	    break
	}
	lappend stripefills [lindex [lindex $fillwidths $fidx] 0]
	set xtot [+ $xtot $fmax]
	# puts stdout "Added to stripe filler [lindex [lindex $fillwidths $fidx] 0]"
	# puts stdout "Total stripe width now $xtot]
    }     
    # Reduce final stripe width to xtot so that it is equal to the
    # best compatible fill size and is a multiple of the unit cell
    # size.
    set stripewidth $xtot

    # If pitchx was given in the tech LEF file, then use that as the
    # minimum pitch to snap values to.  If not, use the minimum fill
    # cell size.
    if {$pitchx > 0} {set fmin $pitchx}

    # Reduce stripepitch to a multiple of the minimum cell size
    # (note:  should change this to a multiple of track pitch)
    set nw [expr int($stripepitch / $fmin)]
    set stripepitch [* $nw $fmin]

    # How many stripes need to be added?  Allow +/-50% (max) to
    # accomodate layout widths that are not a multiple of the
    # stripe pitch.  "numstripes" does not include power stripes
    # outside of the track area on right and left sides.

    if $dostretch {
	set numstripes [expr int($rowmax / $stripepitch) - 1]
	while true {
	    set rowmaxeff [+ $rowmax [* $numstripes $stripewidth]]
	    set newstripes [expr int(($rowmaxeff + 0.5 * $stripepitch) / $stripepitch) - 1]
	    if {$newstripes == $numstripes} {break}
	    set numstripes $newstripes
	}
	if {$numstripes == 0} {
	    set stripewidth 0
	    set dostripes false
	    puts stdout "addspacers:  No room for stripes, none added."
	} else {
	    set stripeoffset [expr $stripepitch - (($stripepitch * ($numstripes + 1)) - $rowmax) / 2]
	    set nw [expr int($stripeoffset / $fmin)]
	    set stripeoffset [* $nw $fmin]

	    # Number of components increases by number of fill cells per stripe *
	    # number of stripes * number of rows.
	    set numfills [* [llength $stripefills] $numstripes $numrows]
	    set numcomps [+ $numcomps $numfills]
	    puts stdout "addspacers:  Inserting $numstripes stripes of width [/ $stripewidth $units] um ($stripewidthreq um requested)"
	    puts stdout "  Pitch [/ $stripepitch $units] um, offset [/ $stripeoffset $units] um"
	}
	puts stdout "stretch:  Number of components is $numcomps"
    } else {
	set numstripes [expr int(($rowmax + 0.5 * $stripepitch) / $stripepitch) - 1]
	if {$numstripes == 0} {
	    set stripewidth 0
	    set dostripes false
	    puts stdout "addspacers:  No room for stripes, none added."
	} else {
	    set stripeoffset [expr $stripepitch - (($stripepitch * ($numstripes + 1)) - $rowmax) / 2]
	    set nw [expr int($stripeoffset / $fmin)]
	    set stripeoffset [* $nw $fmin]
	    puts stdout "addspacers:  Inserting $numstripes stripes of width [/ $stripewidth $units] um ($stripewidthreq um requested)"
	    puts stdout "  Pitch [/ $stripepitch $units] um, offset [/ $stripeoffset $units] um"
	}
    }

    # Analyze design for the number of power bus stripes to add

    # Duplicate "stripepattern" to (at least) the length of (numstripes + 2)
    set pattern ""
    while {[string length $pattern] < [+ $numstripes 2]} {
	append pattern $stripepattern
    }
    set stripepattern $pattern
}

# Diagnostic
puts stdout "Analysis of DEF file:"
puts stdout "Number of components = $number"
puts stdout "New number of components = $numcomps"
puts stdout "Number of rows = [llength [dict keys $rows]]"

set fdef [open $defname r]

if [catch {open $defoutname w} fanno] {
    puts stderr "Error: can't open file $defoutname for output"
    return
}

#-----------------------------------------------------------------
# Read the DEF file a second time to get the number of rows and the length
# of each row
#-----------------------------------------------------------------

# Each stripe has a record of center point and width of free space from
# cell top to bottom (list over rows)
set stripeinfo {}
# Each row has a record of X position of the stripe fill (list over stripes).
set rowstripeinfo {}

set needspecial $dostripes

if {$dostretch == false} {
    # If not stretching the area under the power stripes with fill, the
    # stripeinfo record needs to be filled with the position of each
    # stripe so that write_special gets this information

    set xpos $stripeoffset
    set ns 0
    while {$ns < $numstripes} {
	set sinfo $xpos
	lappend sinfo [- $xpos [/ $stripewidth 2.0]]
	lappend sinfo [+ $xpos [/ $stripewidth 2.0]]
	lappend stripeinfo $sinfo
	set xpos [+ $xpos $stripepitch]
	incr ns
    }
}

while {[gets $fdef line] >= 0} {
    if [regexp {[ \t]*#} $line lmatch] {
	continue
    } elseif [regexp {[ \t]*DIEAREA[ \t]*\([ \t]*([^ \t]+)[ \t]+([^ \t]+)[ \t]*\)[ \t]*\([ \t]*([^ \t]+)[ \t]+([^ \t]+)[ \t]*\)[ \t]*;} $line lmatch \
		diexlow dieylow diexhigh dieyhigh] {

	# Add stripe width to die area total
	if $dostretch {
	    set diexhigh [+ $diexhigh [* $stripewidth $numstripes]]
	    puts $fanno "DIEAREA ( $diexlow $dieylow ) ( $diexhigh $dieyhigh ) ;"
	} else {
	    puts $fanno $line
	}

    } elseif [regexp {[ \t]*TRACKS[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+DO[ \t]+([^ \t]+)[ \t]+STEP[ \t]+([^ \t]+)[ \t]+LAYER[ \t]+([^ \t]+)[ \t]*;} $line lmatch \
		orient low num step layer] {

	if {$dostretch && ($stripewidth > 0) && ($orient == "X")} {
	    # Add number of tracks to cover the added area of the stripes
	    set ntracks [expr int(($stripewidth * $numstripes) / $step)]
	    set num [+ $num $ntracks]
	    puts $fanno "TRACKS X $low DO $num STEP $step LAYER $layer ;"
	} else {
	    puts $fanno $line
	}

    } elseif [regexp {[ \t]*PINS[ \t]+([^ \t]+)[ \t]*;} $line lmatch number] {

	# Add space to pins to match the addition of stripes
	set extrapins false
	if {$dostripes} {

	    # Note:  This duplicates code in write_special.  Needs to be
	    # consolidated into one routine.

	    set tnum 0
	    foreach layer $layers {
		if {[dict get $layer type] != "ROUTING"} {continue}
		if {[dict get $layer direction] == "VERTICAL"} {
		    set lnum [dict get $layer number]
		    if {$lnum > $tnum} {
			set tnum $lnum
			set toplayer [dict get $layer name]
		    }
		}
	    }
	    if {$tnum >= 3} {set extrapins true}
	}
	if {$extrapins} {

	    puts $fanno "PINS [+ $number 2] ;"
	    set i 0
	    set donepower false
	    set doneground false
	    set fillmacro [lindex $stripefills 0]
	    foreach stripe ${stripeinfo} {

		set stripetype [string index $stripepattern $i]
		set smean [lindex $stripe 0]
		set slow [lindex $stripe 1]
		set shigh [lindex $stripe 2]
		if {[- $shigh $slow] > 0} {
		    set smean [/ [+ $shigh $slow] 2.0]
		}
		set nw [expr int($smean / $fmin)]
		set smean [* $nw $fmin]
		set hw [/ $stripewidth 2.0]
		set qw [/ $hw 2.0]
		set slow [- $smean $hw]
		set shigh [+ $smean $hw]
		set cy [+ $dieylow $qw]

		if {!$donepower && ($stripetype == "P")} {
		    set powername [get_power $fillmacro]
		    puts $fanno "- $powername + NET $powername"
		    puts $fanno "  + LAYER $toplayer ( -$hw -$qw ) ( $hw $qw )"
		    puts $fanno "  + PLACED ( $smean $cy ) N ;"
		    set donepower true
		} elseif {!$doneground && ($stripetype == "G")} {
		    set groundname [get_ground $fillmacro]
		    puts $fanno "- $groundname + NET $groundname"
		    puts $fanno "  + LAYER $toplayer ( -$hw -$qw ) ( $hw $qw )"
		    puts $fanno "  + PLACED ( $smean $cy ) N ;"
		    set doneground true
		}
		incr i
		if {$donepower && $doneground} {break}
	    }
	} else {
	    puts $fanno $line
	}

	if {$dostripes && $dostretch} {
	    while {[gets $fdef line] >= 0} {
		if [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch sectiontest] {
		    puts $fanno $line
		    break
		} elseif [regexp {[ \t]*\+[ \t]+PLACED[ \t]+\([ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+\)[ \t]+([^ \t]+)[ \t]+;} $line lmatch \
			px py orient] {

		    # Check if instance px exceeds xbreak and adjust accordingly
		    if {$px > $stripepitch} {
			set ns [expr int($px / $stripepitch)]
			if {$ns > $numstripes} {set ns $numstripes}
			set nw [* $ns $stripewidth]
			set xpos [+ $px $nw]
			puts $fanno "  + PLACED ( $xpos $py ) $orient ;"
		    } else {
			puts $fanno $line
		    }
		} else {
		    puts $fanno $line
		}
	    }
	}

    } elseif [regexp {[ \t]*COMPONENTS[ \t]+([^ \t]+)[ \t]*;} $line lmatch number] {
	puts $fanno "COMPONENTS $numcomps ;"
	set r 0
	set s 0
	set xadd 0
	if {$dostripes && $dostretch} {
	    set xbreak $stripeoffset
	} else {
	    set xbreak $rowmax
	}
	while {[gets $fdef line] >= 0} {
	    if [regexp {[ \t]*END[ \t]+([^ \t]+)[ \t]*$} $line lmatch sectiontest] {
		puts $fanno $line

		if {$dostripes} {
		    # Finish computing values of internal stripes
		    # replace total with mean, and copy the bounds values
		    set newinfo {}
		    foreach stripe ${stripeinfo} {
			set smean [lindex $stripe 0]
			set slow [lindex $stripe 1]
			set shigh [lindex $stripe 2]
			set smean [/ $smean $numrows]
			lappend newinfo [list $smean $slow $shigh]
		    }
		    set stripeinfo $newinfo
		}
		break

	    } elseif [regexp {[ \t]*-[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+\+[ \t]+PLACED[ \t]+\([ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+\)[ \t]+([^ \t]+)[ \t]+;} $line lmatch \
			instance macro px py orient] {

		# Quick check if the right side of instance exceeds xbreak and
		# if placing the stripe before the instance will put the stripe
		# closer to the ideal position.

		set pw [set ${macro}(w)]
		set right [+ $px $pw]

		# Check if instance position px exceeds xbreak and adjust accordingly
		if {($px >= $xbreak) || ([- $xbreak $px] < [- [+ $px $pw] $xbreak])} {

		    if {[llength $rowstripeinfo] < $numstripes} {

			# Add to row stripe info
			lappend rowstripeinfo [+ $px $xadd]

			# Add fill in the stripe area
			set i 0
			foreach fmacro $stripefills {
			    set fw [set ${fmacro}(w)]
			    set xpos [+ $px $xadd]
			    puts $fanno "- ${fmacro}_${r}_${s}_${i} ${fmacro} + PLACED ( $xpos $py ) $orient ;"
			    set xadd [+ $xadd $fw]
			    incr i
			}
			if {$s < $numstripes} {
			    set xbreak [+ $xbreak $stripepitch]
			    incr s
			} else {
			    set xbreak $rowmax
			}
		    }
		}
		set px [+ $px $xadd] 
		puts $fanno "- $instance $macro + PLACED ( $px $py ) $orient ;"

		# Check if there is a match to the last instance in the row
		set rowvals [dict get $rows $py]
		set rowinst [lindex $rowvals 1]
		if {[string equal $instance $rowinst]} {
		    incr r
		    set xpos [lindex $rowvals 0]
		    if {$dostripes && $dostretch} {
			# puts stdout "stripewidth = $stripewidth"
			# puts stdout "numstripes = $numstripes"
			# puts stdout "xpos in = $xpos"
			set xpos [+ $xpos [* $stripewidth $numstripes]]
			# puts stdout "xpos out = $xpos"
		    }
		    set fills [lindex $rowvals 4]
		    # Get orientation of row (N or S);
		    # remove "F" if last cell was flipped
		    set orient [string index [lindex $rowvals 3] end]
		    foreach fpair $fills {
			set fmacro [lindex $fpair 0]
			set fw [set ${fmacro}(w)]
			set fn [lindex $fpair 1]
			for {set i 1} {$i <= $fn} {incr i} {
			    puts $fanno "- ${fmacro}_${r}_${i} ${fmacro} + PLACED ( $xpos $py ) $orient ;"
			    set xpos [+ $xpos $fw]
			}
		    }
		    # Add rowstripeinfo values to stripeinfo
		    # Reset xbreak, xadd, and rowstripeinfo
		    if {$dostretch && ($xadd > 0)} {
			set xadd 0
			set xbreak $stripeoffset
			set i 0
			set newstripeinfo {}
			if {[llength $stripeinfo] == 0} {
			    foreach sx $rowstripeinfo {
				set slow $sx
				set shigh [+ $sx $stripewidth]
				set smean [+ $sx [/ $stripewidth 2.0]]
				lappend newstripeinfo [list $smean $slow $shigh]
			    }
			} else {
			    foreach sx $rowstripeinfo {
				set sinfo [lindex $stripeinfo $i]
				set slow  [max [lindex $sinfo 1] $sx]
				set shigh [min [lindex $sinfo 2] [+ $sx $stripewidth]]
				set smean [+ [lindex $sinfo 0] [+ $sx [/ $stripewidth 2.0]]]
				lappend newstripeinfo [list $smean $slow $shigh]
				incr i
			    }
			}
			set stripeinfo $newstripeinfo
			set rowstripeinfo {}
			set s 0
		    }
		}
 	    }
	}
    } elseif [regexp {[ \t]*END[ \t]+DESIGN} $line lmatch] {
	if {$needspecial == true} {
	    puts $fanno "SPECIALNETS 2 ;"
	    write_special $stripeinfo $stripefills $stripewidth $stripepattern \
			$rows $dieylow $dieyhigh $fanno
	    puts $fanno "END SPECIALNETS"
	}
	puts $fanno $line
    } elseif [regexp {[ \t]*SPECIALNETS[ \t]+([^ \t]+)} $line lmatch netnums] {
	if {$needspecial == true} {
	    puts $fanno "SPECIALNETS [+ 2 $netnums] ;"
	    write_special $stripeinfo $stripefills $stripewidth $stripepattern \
			$rows $dieylow $dieyhigh $fanno
	    set needspecial false
	}
    } else {
	puts $fanno $line
    }
}

close $fanno
close $fdef
if {$techlef != ""} {
    close $ftech
}

#-----------------------------------------------------------------
# If there is a ${topname}.obs file, read it in and adjust the
# X values to account for the spacers.
#-----------------------------------------------------------------

if {$dostripes && $dostretch} {

    # Cast values back into microns, which are the units used by the .obs file
    set stripewidth [/ $stripewidth $units]
    set stripepitch [/ $stripepitch $units]
    set stripeoffset [/ $stripeoffset $units]

    if [catch {open ${topname}.obs r} infobs] {
	# No .obs file, no action needed.
	puts stdout "No file ${topname}.obs, so no adjustment required."
	return
    }
    if [catch {open ${topname}.obsx w} outfobs] {
	puts stout "Error:  Cannot open file ${topname}.obsx for writing!"
	return
    }
    puts stdout "Adjusting obstructions for power striping"
    # puts stdout "numstripes = $numstripes"
    # puts stdout "stripewidth = $stripewidth um"
    # puts stdout "stripepitch = $stripepitch um"
    # puts stdout "stripeoffset = $stripeoffset um"
    while {[gets $infobs line] >= 0} {
	if [regexp {[ \t]*obstruction[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]+([^ \t]+)[ \t]*$} $line \
			lmatch xlow ylow xhigh yhigh layer] {
            # puts stdout "In: $xlow $ylow $xhigh $yhigh $layer"
	    if {$xlow > $stripeoffset} {
		set ns [expr int(($xlow - $stripeoffset + $stripepitch) / $stripepitch)]
		if {$ns > $numstripes} {set ns $numstripes}
		set nw [* $ns $stripewidth]
		set xlow [+ $xlow $nw]
		# puts stdout "xlow adjust ns = $ns nw = $nw"
	    } 
	    if {$xhigh > $stripeoffset} {
		set ns [expr int(($xhigh - $stripeoffset + $stripepitch) / $stripepitch)]
		if {$ns > $numstripes} {set ns $numstripes}
		set nw [* $ns $stripewidth]
		set xhigh [+ $xhigh $nw]
		# puts stdout "xhigh adjust ns = $ns nw = $nw"
	    }
            # puts stdout "Out: $xlow $ylow $xhigh $yhigh $layer"
	    puts $outfobs "obstruction $xlow $ylow $xhigh $yhigh $layer"
	} else {
	    puts $outfobs $outline
	}
    }
    close $outfobs
    close $infobs
}

puts stdout "Done with addspacers.tcl"
