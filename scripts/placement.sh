#!/bin/tcsh -f
#----------------------------------------------------------
# Placement script using GrayWolf
#
# This script assumes the existence of the pre-GrayWolf
# ".cel" and ".par" files.  It will run GrayWolf for the
# placement.
#----------------------------------------------------------
# Tim Edwards, 5/16/11, for Open Circuit Design
# Modified April 2013 for use with qflow
# Modified November 2013 for congestion feedback
#----------------------------------------------------------

if ($#argv < 2) then
   echo Usage:  placement.sh <project_path> <source_name>
   exit 1
endif

# Split out options from the main arguments
set argline=(`getopt "kdf" $argv[1-]`)
set cmdargs=`echo "$argline" | awk 'BEGIN {FS = "-- "} END {print $2}'`
set argc=`echo $cmdargs | wc -w`

if ($argc == 2) then
   set argv1=`echo $cmdargs | cut -d' ' -f1`
   set argv2=`echo $cmdargs | cut -d' ' -f2`
else
   echo Usage:  placement.sh [options] <project_path> <source_name>
   echo   where
   echo       <project_path> is the name of the project directory containing
   echo                 a file called qflow_vars.sh.
   echo       <source_name> is the root name of the verilog file, and
   echo       [options] are:
   echo			-k	keep working files
   echo			-d	generate DEF file for routing
   echo			-f	final placement.  Don't run if routing succeeded
   echo
   echo	  Options to specific tools can be specified with the following
   echo	  variables in project_vars.sh:
   echo
   echo
   exit 1
endif

set keep=0
set makedef=0
set final = 0

foreach option (${argline})
   switch (${option})
      case -k:
         set keep=1
         breaksw
      case -d:
         set makedef=1
         breaksw
      case -f:
         set final=1
	 breaksw
      case --:
         break
   endsw
end

set projectpath=$argv1
set sourcename=$argv2
set rootname=${sourcename:h}

# This script is called with the first argument <project_path>, which should
# have file "qflow_vars.sh".  Get all of our standard variable definitions
# from the qflow_vars.sh file.

if (! -f ${projectpath}/qflow_vars.sh ) then
   echo "Error:  Cannot find file qflow_vars.sh in path ${projectpath}"
   exit 1
endif

source ${projectpath}/qflow_vars.sh
source ${techdir}/${techname}.sh
cd ${projectpath}
if (-f project_vars.sh) then
   source project_vars.sh
endif

# Prepend techdir to leffile unless leffile begins with "/"
set abspath=`echo ${leffile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set lefpath=${leffile}
else
   set lefpath=${techdir}/${leffile}
endif

# Prepend techdir to techleffile unless techleffile begins with "/"
set abspath=`echo ${techleffile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set techlefpath=${techleffile}
else
   set techlefpath=${techdir}/${techleffile}
endif

# Prepend techdir to spicefile unless spicefile begins with "/"
set abspath=`echo ${spicefile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set spicepath=${spicefile}
else
   set spicepath=${techdir}/${spicefile}
endif

mkdir -p ${logdir}
set lastlog=${logdir}/synth.log
set synthlog=${logdir}/place.log
rm -f ${synthlog} >& /dev/null
rm -f ${logdir}/sta.log >& /dev/null
rm -f ${logdir}/route.log >& /dev/null
rm -f ${logdir}/post_sta.log >& /dev/null
touch ${synthlog}
set date=`date`
echo "Qflow placement logfile created on $date" > ${synthlog}

#----------------------------------------------------------
# Done with initialization
#----------------------------------------------------------

# Check if last line of log file says "error condition"
set errcond = `tail -1 ${lastlog} | grep "error condition" | wc -l`
if ( ${errcond} == 1 ) then
   echo "Synthesis flow stopped on error condition.  Placement will not proceed"
   echo "until error condition is cleared."
   exit 1
endif

#-------------------------------------------------------------------------
# Create the .cel file for GrayWolf
#-------------------------------------------------------------------------

cd ${projectpath}

echo "Running blif2cel.tcl" |& tee -a ${synthlog}

set blif2cel_commands = ""

if ( ${?hard_cells} ) then
	foreach cell (${hard_cells})
		set blif2cel_commands = "${blif2cel_commands} --hard-macro ${cell}"
	end
endif

if ( ${?pad_config} ) then
	set blif2cel_commands = "${blif2cel_commands} --pad-config ${pad_config}"
endif

if ( ${?vddnet} ) then
	set blif2cel_commands = "${blif2cel_commands} --vdd-net ${vddnet}"
endif

if ( ${?gndnet} ) then
	set blif2cel_commands = "${blif2cel_commands} --gnd-net ${gndnet}"
endif

if ( ${?pad_width} ) then
	set blif2cel_commands = "${blif2cel_commands} --pad-width ${pad_width}"
endif

if ( ${?pad_height} ) then
	set blif2cel_commands = "${blif2cel_commands} --pad-height ${pad_height}"
endif

${scriptdir}/blif2cel.tcl \
	--blif ${synthdir}/${rootname}.blif\
	${blif2cel_commands} --lef ${lefpath}\
	--cel ${layoutdir}/${rootname}.cel >>& ${synthlog}

#---------------------------------------------------------------------
# Spot check:  Did blif2cel produce file ${rootname}.cel?
#---------------------------------------------------------------------

if ( !( -f ${layoutdir}/${rootname}.cel || ( -M ${layoutdir}/${rootname}.cel \
	< -M ${rootname}.blif ))) then
   echo "blif2cel failure:  No file ${rootname}.cel." |& tee -a ${synthlog}
   echo "blif2cel was called with arguments: ${synthdir}/${rootname}.blif "
   echo "      ${lefpath} ${layoutdir}/${rootname}.cel"
   echo "Premature exit." |& tee -a ${synthlog}
   echo "Synthesis flow stopped due to error condition." >> ${synthlog}
   exit 1
endif

#-------------------------------------------------------------------------
# If placement option "initial_density" is set, run the decongest
# script.  This will annotate the .cel file with fill cells to pad
# out the area to the specified density.
#-------------------------------------------------------------------------

cd ${layoutdir}

if ( ${?initial_density} ) then
   echo "Running decongest to set initial density of ${initial_density}"
   ${scriptdir}/decongest.tcl ${rootname} ${lefpath} \
		${fillcell} ${initial_density} |& tee -a ${synthlog}
   cp ${rootname}.cel ${rootname}.cel.bak
   mv ${rootname}.acel ${rootname}.cel
endif

# Check if a .acel file exists.  This file is produced by qrouter and
# its existance indicates that qrouter is passing back congestion
# information to GrayWolf for a final placement pass using fill to
# break up congested areas.

if ( -f ${rootname}.acel && ( -M ${rootname}.acel >= -M ${rootname}.cel )) then
   cp ${rootname}.cel ${rootname}.cel.bak
   mv ${rootname}.acel ${rootname}.cel
   set final = 1
else
   if ( ${final} == 1 ) then
      # Called for final time, but routing already succeeded, so just exit
      echo "First attempt routing succeeded;  final placement iteration is unnecessary."
      exit 2
   endif
endif

# Check if a .cel2 file exists and needs to be appended to .cel
# If the .cel2 file is newer than .cel, then truncate .cel and
# re-append.

if ( -f ${rootname}.cel2 ) then
   echo "Preparing pin placement hints from ${rootname}.cel2" |& tee -a ${synthlog}
   if ( `grep -c padgroup ${rootname}.cel` == "0" ) then
      cat ${rootname}.cel2 >> ${rootname}.cel
   else if ( -M ${rootname}.cel2 > -M ${rootname}.cel ) then
      # Truncate .cel file to first line containing "padgroup"
      cat ${rootname}.cel | sed -e "/padgroup/Q" > ${rootname}_tmp.cel
      cat ${rootname}_tmp.cel ${rootname}.cel2 > ${rootname}.cel
      rm -f ${rootname}_tmp.cel
   endif
else
   echo -n "No ${rootname}.cel2 file found for project. . . " \
		|& tee -a ${synthlog}
   echo "continuing without pin placement hints" |& tee -a ${synthlog}
endif

# Add fill cells for the power bus stripes
# (this is work in progress, commented out for now)

# echo "Running powerbus to add spacers for power bus stripes" |& tee -a ${synthlog}
# ${scriptdir}/powerbus.tcl ${rootname} ${lefpath} ${fillcell} |& tee -a ${synthlog}

# powerbus.tcl creates a .acel file if successful.  If not, then
# leave the .cel file in place

if ( -f ${rootname}.acel && ( -M ${rootname}.acel >= -M ${rootname}.cel )) then
   cp ${rootname}.cel ${rootname}.cel.bak
   mv ${rootname}.acel ${rootname}.cel
endif

#-----------------------------------------------
# 1) Run GrayWolf
#-----------------------------------------------

if ( !( ${?graywolf_options} )) then
   if ( !( ${?DISPLAY} )) then
      set graywolf_options = "-n"
   else
      set graywolf_options = ""
   endif
endif

echo "Running GrayWolf placement" |& tee -a ${synthlog}
   ${bindir}/graywolf ${graywolf_options} $rootname >>& ${synthlog}
endif

#---------------------------------------------------------------------
# Spot check:  Did GrayWolf produce file ${rootname}.pin?
#---------------------------------------------------------------------

if ( !( -f ${rootname}.pin || ( -M ${rootname}.pin < -M ${rootname}.cel ))) then
   echo "GrayWolf failure:  No file ${rootname}.pin." |& tee -a ${synthlog}
   echo "Premature exit." |& tee -a ${synthlog}
   echo "Synthesis flow stopped due to error condition." >> ${synthlog}
   exit 1
endif

#---------------------------------------------------
# 2) Prepare DEF and .cfg files for qrouter
#---------------------------------------------------

# First prepare a simple .cfg file that can be used to point qrouter
# to the LEF files when generating layer information using the "-i" option.

if ($makedef == 1) then

   #------------------------------------------------------------------
   # Determine the version number and availability of scripting
   # in qrouter.
   #------------------------------------------------------------------

   set version=`${bindir}/qrouter -v 0 -h | tail -1`
   set major=`echo $version | cut -d. -f1`
   set minor=`echo $version | cut -d. -f2`
   set subv=`echo $version | cut -d. -f3`
   set scripting=`echo $version | cut -d. -f4`

   # Create the initial (bootstrap) configuration file

   if ( $scripting == "T" ) then
      if ( "$techleffile" == "" ) then
         echo "read_lef ${lefpath}" > ${rootname}.cfg
      else
         echo "read_lef ${techlefpath}" > ${rootname}.cfg
      endif
   else
      if ( "$techleffile" == "" ) then
         echo "lef ${lefpath}" > ${rootname}.cfg
      else
         echo "lef ${techlefpath}" > ${rootname}.cfg
      endif
   endif

   ${bindir}/qrouter -i ${rootname}.info -c ${rootname}.cfg

   #---------------------------------------------------------------------
   # Spot check:  Did qrouter produce file ${rootname}.info?
   #---------------------------------------------------------------------

   if ( !( -f ${rootname}.info || ( -M ${rootname}.info < -M ${rootname}.pin ))) then
      echo "qrouter (-i) failure:  No file ${rootname}.info." |& tee -a ${synthlog}
      echo "Premature exit." |& tee -a ${synthlog}
      echo "Synthesis flow stopped due to error condition." >> ${synthlog}
      exit 1
   endif

   # Run getfillcell to determine which cell should be used for fill to
   # match the width specified for feedthroughs in the .par file.  If
   # nothing is returned by getfillcell, then either feedthroughs have
   # been disabled, or else we'll try passing $fillcell directly to
   # place2def

   echo "Running getfillcell.tcl" |& tee -a ${synthlog}
   set usefillcell = `${scriptdir}/getfillcell.tcl $rootname \
	${lefpath} $fillcell | grep fill= | cut -d= -f2`

   if ( "${usefillcell}" == "" ) then
      set usefillcell = $fillcell
   endif
   echo "Using cell ${usefillcell} for fill" |& tee -a ${synthlog}

   # Run place2def to turn the GrayWolf output into a DEF file

   if ( ${?route_layers} ) then
      ${scriptdir}/place2def.tcl $rootname $usefillcell ${route_layers} \
		 >>& ${synthlog}
   else
      ${scriptdir}/place2def.tcl $rootname $usefillcell >>& ${synthlog}
   endif

   #---------------------------------------------------------------------
   # Spot check:  Did place2def produce file ${rootname}.def?
   #---------------------------------------------------------------------

   if ( !( -f ${rootname}.def || ( -M ${rootname}.def < -M ${rootname}.pin ))) then
      echo "place2def failure:  No file ${rootname}.def." |& tee -a ${synthlog}
      echo "Premature exit." |& tee -a ${synthlog}
      echo "Synthesis flow stopped due to error condition." >> ${synthlog}
      exit 1
   endif

   #---------------------------------------------------------------------
   # Add spacer cells to create a straight border on the right side
   #---------------------------------------------------------------------

   if ( -f ${scriptdir}/addspacers.tcl ) then

      if ( !( ${?addspacers_options} )) then
         set addspacers_options = ""
      endif

      if ( "$techleffile" != "" ) then
         set addspacers_options = "-techlef ${techlefpath} ${addspacers_options}"
      endif

      echo "Running addspacers.tcl ${addspacers_options} ${rootname} ${lefpath} ${fillcell}" |& tee -a ${synthlog}

      ${scriptdir}/addspacers.tcl ${addspacers_options} \
		${rootname} ${lefpath} ${fillcell} >>& ${synthlog}
      if ( -f ${rootname}_filled.def ) then
	 mv ${rootname}_filled.def ${rootname}.def
	 # Copy the .def file to a backup called "unroute"
	 cp ${rootname}.def ${rootname}_unroute.def
      endif

      if ( -f ${rootname}.obsx ) then
         # If addspacers annotated the .obs (obstruction) file, then
         # overwrite the original.
	 mv ${rootname}.obsx ${rootname}.obs
      endif
   else
      # Copy the .def file to a backup called "unroute"
      cp ${rootname}.def ${rootname}_unroute.def
   endif

   # If the user didn't specify a number of layers for routing as part of
   # the project variables, then the info file created by qrouter will have
   # as many lines as there are route layers defined in the technology LEF
   # file.

   if ( !( ${?route_layers} )) then
      set route_layers = `cat ${rootname}.info | grep -e horizontal -e vertical | wc -l`
   endif

   # Create the main configuration file

   # Variables "via_pattern" (none, normal, invert) and "via_stacks"
   # can be specified in the tech script, and are appended to the
   # qrouter configuration file.  via_stacks defaults to 2 if not
   # specified.  It can be overridden from the user's .cfg2 file.

   if (${scripting} == "T") then
      echo "# qrouter runtime script for project ${rootname}" > ${rootname}.cfg
      echo "" >> ${rootname}.cfg
      echo "verbose 1" >> ${rootname}.cfg
      if ( "$techleffile" != "" ) then
         echo "read_lef ${techlefpath}" >> ${rootname}.cfg
      endif
      echo "read_lef ${lefpath}" >> ${rootname}.cfg
      echo "catch {layers ${route_layers}}" >> ${rootname}.cfg
      if ( ${?via_pattern} ) then
         echo "" >> ${rootname}.cfg
         echo "via pattern ${via_pattern}" >> ${rootname}.cfg
      endif
      if (! ${?via_stacks} ) then
         set via_stacks=2
         echo "via stack ${via_stacks}" >> ${rootname}.cfg
      endif
      if ( ${?vddnet} ) then
	 echo "vdd $vddnet" >> ${rootname}.cfg
      endif
      if ( ${?gndnet} ) then
	 echo "gnd $gndnet" >> ${rootname}.cfg
      endif

   else
      echo "# qrouter configuration for project ${rootname}" > ${rootname}.cfg
      echo "" >> ${rootname}.cfg
      if ( "$techleffile" != "" ) then
         echo "lef ${techlefpath}" >> ${rootname}.cfg
      endif
      echo "lef ${lefpath}" >> ${rootname}.cfg
      echo "num_layers ${route_layers}" >> ${rootname}.cfg
      if ( ${?via_pattern} ) then
         echo "" >> ${rootname}.cfg
         echo "via pattern ${via_pattern}" >> ${rootname}.cfg
      endif
      if (! ${?via_stacks} ) then
         set via_stacks=2
         echo "stack ${via_stacks}" >> ${rootname}.cfg
      endif
   endif

   # Add obstruction fence around design, created by place2def.tcl
   # and modified by addspacers.tcl

   if ( -f ${rootname}.obs ) then
      cat ${rootname}.obs >> ${rootname}.cfg
   endif

   # Scripted version continues with the read-in of the DEF file

   if (${scripting} == "T") then
      echo "read_def ${rootname}.def" >> ${rootname}.cfg
   endif

   # If there is a file called ${rootname}.cfg2, then append it to the
   # ${rootname}.cfg file.  It will be used to define all routing behavior.
   # Otherwise, if using scripting, then append the appropriate routing
   # command or procedure based on whether this is a pre-congestion
   # estimate of routing or the final routing pass.

   if ( -f ${rootname}.cfg2 ) then
      cat ${rootname}.cfg2 >> ${rootname}.cfg
   else
      if (${scripting} == "T") then
	 echo "qrouter::standard_route ${rootname}_route.def false" >> ${rootname}.cfg
	 echo "qrouter::write_delays ${rootname}.rc" >> ${rootname}.cfg
	 # Standard_route's automatic quit has been subverted in order
	 # to write the delay file, so make sure that qrouter actually exits.
	 echo "quit" >> ${rootname}.cfg
      endif
   endif

   #------------------------------------------------------------------
   # Automatic optimization of buffer tree placement causes the
   # original BLIF netlist, with tentative buffer assignments, to
   # be invalid.  Use the blifanno.tcl script to back-annotate the
   # correct assignments into the original BLIF netlist, then
   # use that BLIF netlist to regenerate the SPICE and RTL verilog
   # netlists.
   #------------------------------------------------------------------

   ${scriptdir}/blifanno.tcl ${synthdir}/${rootname}.blif ${rootname}.def \
		${synthdir}/${rootname}_anno.blif >>& ${synthlog}

   #------------------------------------------------------------------
   # Spot check:  Did blifanno.tcl produce an output file?
   #------------------------------------------------------------------

   if ( !( -f ${synthdir}/${rootname}_anno.blif )) then
      echo "blifanno.tcl failure:  No file ${rootname}_anno.blif." \
		|& tee -a ${synthlog}
      echo "RTL verilog and SPICE netlists may be invalid if there" \
		|& tee -a ${synthlog}
      echo "were buffer trees optimized by placement." |& tee -a ${synthlog}
      echo "Synthesis flow continuing, condition not fatal." >> ${synthlog}
    else
      echo "" >> ${synthlog}
      echo "Generating RTL verilog and SPICE netlist file in directory" \
		|& tee -a ${synthlog}
      echo "   ${synthdir}" |& tee -a ${synthlog}
      echo "Files:" |& tee -a ${synthlog}
      echo "   Verilog: ${synthdir}/${rootname}.rtl.v" |& tee -a ${synthlog}
      echo "   Verilog: ${synthdir}/${rootname}.rtlnopwr.v" |& tee -a ${synthlog}
      echo "   Spice:   ${synthdir}/${rootname}.spc" |& tee -a ${synthlog}
      echo "" >> ${synthlog}

      cd ${synthdir}

      #------------------------------------------------------------------
      # Copy the original rtl.v and rtlnopwr.v for use in comparison of
      # pre- and post-placement netlists.
      #------------------------------------------------------------------

      echo "Copying ${rootname}.rtl.v and ${rootname}.rtlnopwr.v to backups"
      cp ${rootname}.rtl.v ${rootname}_synth.rtl.v
      cp ${rootname}.rtlnopwr.v ${rootname}_synth.rtlnopwr.v

      echo "Running blif2Verilog." |& tee -a ${synthlog}
      ${bindir}/blif2Verilog -c -v ${vddnet} -g ${gndnet} \
		${rootname}_anno.blif > ${rootname}.rtl.v

      ${bindir}/blif2Verilog -c -p -v ${vddnet} -g ${gndnet} \
		${rootname}_anno.blif > ${rootname}.rtlnopwr.v

      echo "Running blif2BSpice." |& tee -a ${synthlog}
      ${bindir}/blif2BSpice -i -p ${vddnet} -g ${gndnet} -l \
		${spicepath} ${rootname}_anno.blif \
		> ${rootname}.spc

      #------------------------------------------------------------------
      # Spot check:  Did blif2Verilog or blif2BSpice exit with an error?
      #------------------------------------------------------------------

      if ( !( -f ${rootname}.rtl.v || \
		( -M ${rootname}.rtl.v < -M ${rootname}.blif ))) then
	 echo "blif2Verilog failure:  No file ${rootname}.rtl.v created." \
		|& tee -a ${synthlog}
      endif

      if ( !( -f ${rootname}.rtlnopwr.v || \
		( -M ${rootname}.rtlnopwr.v < -M ${rootname}.blif ))) then
	 echo "blif2Verilog failure:  No file ${rootname}.rtlnopwr.v created." \
		|& tee -a ${synthlog}
      endif

      if ( !( -f ${rootname}.spc || \
		( -M ${rootname}.spc < -M ${rootname}.blif ))) then
	 echo "blif2BSpice failure:  No file ${rootname}.spc created." \
		|& tee -a ${synthlog}
      endif

      # Return to the layout directory
      cd ${layoutdir}

    endif
endif

#---------------------------------------------------
# 4) Remove working files (except for the main
#    output files .pin, .pl1, and .pl2
#---------------------------------------------------

if ($keep == 0) then
   rm -f ${rootname}.blk ${rootname}.gen ${rootname}.gsav ${rootname}.history
   rm -f ${rootname}.log ${rootname}.mcel ${rootname}.mdat ${rootname}.mgeo
   rm -f ${rootname}.mout ${rootname}.mpin ${rootname}.mpth ${rootname}.msav
   rm -f ${rootname}.mver ${rootname}.mvio ${rootname}.stat ${rootname}.out
   rm -f ${rootname}.pth ${rootname}.sav ${rootname}.scel ${rootname}.txt
endif

#------------------------------------------------------------
# Done!
#------------------------------------------------------------

set endtime = `date`
echo "Placement script ended on $endtime" >> $synthlog

exit 0
