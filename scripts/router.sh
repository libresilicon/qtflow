#!/bin/tcsh -f
#----------------------------------------------------------
# Route script using qrouter
#----------------------------------------------------------
# Tim Edwards, 5/16/11, for Open Circuit Design
# Modified April 2013 for use with qflow
#----------------------------------------------------------

if ($#argv < 2) then
   echo Usage:  router.sh [options] <project_path> <source_name>
   exit 1
endif

# Split out options from the main arguments
set argline=(`getopt "nr" $argv[1-]`)

set options=`echo "$argline" | awk 'BEGIN {FS = "-- "} END {print $1}'`
set cmdargs=`echo "$argline" | awk 'BEGIN {FS = "-- "} END {print $2}'`
set argc=`echo $cmdargs | wc -w`

if ($argc >= 2) then
   set argv1=`echo $cmdargs | cut -d' ' -f1`
   set argv2=`echo $cmdargs | cut -d' ' -f2`
   if ($argc == 3) then
      set statusin = `echo $cmdargs | cut -d' ' -f3`
      if ($statusin == 2) then
	 echo "Qrouter completed on first iteration, no need to run again."
         exit 0
      endif
   endif
else
   echo Usage:  router.sh [options] <project_path> <source_name>
   echo   where
   echo       <project_path> is the name of the project directory containing
   echo                 a file called qflow_vars.sh.
   echo       <source_name> is the root name of the verilog file
   exit 1
endif

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

if (! ${?qrouter_options} ) then
   set qrouter_options = ${options}
   if (!($?route_show)) then
      set qrouter_options = "-noc ${qrouter_options}"
   endif
endif

mkdir -p ${logdir}
set lastlog=${logdir}/sta.log
set synthlog=${logdir}/route.log
rm -f ${synthlog} >& /dev/null
rm -f ${logdir}/post_sta.log >& /dev/null
touch ${synthlog}
set date=`date`
echo "Qflow route logfile created on $date" > ${synthlog}


# Check if last line of log file says "error condition"
set errcond = `tail -1 ${lastlog} | grep "error condition" | wc -l`
if ( ${errcond} == 1 ) then
   echo "Synthesis flow stopped on error condition.  Detail routing"
   echo "will not proceed until error condition is cleared."
   exit 1
endif

# Prepend techdir to leffile unless leffile begins with "/"
set abspath=`echo ${leffile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set lefpath=${leffile}
else
   set lefpath=${techdir}/${leffile}
endif

#----------------------------------------------------------
# Done with initialization
#----------------------------------------------------------

cd ${layoutdir}

#------------------------------------------------------------------
# Determine the version number and availability of scripting
#------------------------------------------------------------------

set version=`${bindir}/qrouter -v 0 -h | tail -1`
set major=`echo $version | cut -d. -f1`
set minor=`echo $version | cut -d. -f2`
set subv=`echo $version | cut -d. -f3`
set scripting=`echo $version | cut -d. -f4`

# If there is a file called (project)_unroute.def, copy it
# to the primary .def file to be used by the router.  This
# overwrites any previously generated route solution.

if ( -f ${rootname}_unroute.def ) then
   cp ${rootname}_unroute.def ${rootname}.def
endif

if (${scripting} == "T") then

#------------------------------------------------------------------
# Scripted qrouter.  Given qrouter with Tcl/Tk scripting capability,
# create a script to perform the routing.  The script will allow
# the graphics to display, keep the output to the console at a
# minimum, and generate a file with congestion information in the
# case of route failure.
#------------------------------------------------------------------

   echo "Running qrouter $version"
   ${bindir}/qrouter ${qrouter_options} -s ${rootname}.cfg \
		|& tee -a ${synthlog} | \
		grep - -e fail -e Progress -e remaining.\*00\$ \
		-e remaining:\ \[1-9\]0\\\?\$ -e \\\*\\\*\\\*
else

#------------------------------------------------------------------
# Create the detailed route.  Monitor the output and print errors
# to the output, as well as writing the "commit" line for every
# 100th route, so the end-user can track the progress.
#------------------------------------------------------------------

   echo "Running qrouter $version"
   ${bindir}/qrouter -c ${rootname}.cfg -p ${vddnet} -g ${gndnet} \
		${qrouter_options} ${rootname} |& tee -a ${synthlog} | \
		grep - -e fail -e Progress -e remaining.\*00\$ \
		-e remaining:\ \[1-9\]0\\\?\$ -e \\\*\\\*\\\*
endif

#---------------------------------------------------------------------
# Spot check:  Did qrouter produce file ${rootname}_route.def?
#---------------------------------------------------------------------

if ( !( -f ${rootname}_route.def || ( -M ${rootname}_route.def \
		< -M ${rootname}.def ))) then
   echo "qrouter failure:  No file ${rootname}_route.def." |& tee -a ${synthlog}
   echo "Premature exit." |& tee -a ${synthlog}
   echo "Synthesis flow stopped due to error condition." >> ${synthlog}
   exit 1
endif

#---------------------------------------------------------------------
# If qrouter generated a ".cinfo" file, then annotate the ".cel"
# file, re-run placement, and re-run routing.
#---------------------------------------------------------------------

if (${scripting} == "T") then
   if ( -f ${rootname}.cinfo && ( -M ${rootname}.cinfo \
		> -M ${rootname}.def )) then
      ${scriptdir}/decongest.tcl ${rootname} ${lefpath} \
		${fillcell} |& tee -a ${synthlog}
   endif
endif

if ( -f ${rootname}_route.def ) then
   rm -f ${rootname}.def
   mv ${rootname}_route.def ${rootname}.def
endif

#------------------------------------------------------------
# Done!
#------------------------------------------------------------

set endtime = `date`
echo "Router script ended on $endtime" >> $synthlog

exit 0
