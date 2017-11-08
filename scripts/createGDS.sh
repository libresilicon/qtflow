#!/bin/tcsh -f
#----------------------------------------------------------
# Qflow layout GDS generation script using magic-8.0
#----------------------------------------------------------
# Tim Edwards, April 2013
#----------------------------------------------------------

if ($#argv < 2) then
   echo Usage:  display.sh [options] <project_path> <source_name>
   exit 1
endif

# Split out options from the main arguments
set argline=(`getopt "nr" $argv[1-]`)

set options=`echo "$argline" | awk 'BEGIN {FS = "-- "} END {print $1}'`
set cmdargs=`echo "$argline" | awk 'BEGIN {FS = "-- "} END {print $2}'`
set argc=`echo $cmdargs | wc -w`

if ($argc == 2) then
   set argv1=`echo $cmdargs | cut -d' ' -f1`
   set argv2=`echo $cmdargs | cut -d' ' -f2`
else
   echo Usage:  display.sh [options] <project_path> <source_name>
   echo   where
   echo       <project_path> is the name of the project directory containing
   echo                 a file called qflow_vars.sh.
   echo       <source_name> is the root name of the verilog file, and
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

# Prepend techdir to gdsfile unless gdsfile begins with "/"
set abspath=`echo ${gdsfile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set gdspath=${gdsfile}
else
   set gdspath=${techdir}/${gdsfile}
endif

# Prepend techdir to techfile unless techfile begins with "/"
set abspath=`echo ${techfile} | cut -c1`
if ( "${abspath}" == "/" ) then
   set techpath=${techfile}
else
   set techpath=${techdir}/${techfile}
endif

#----------------------------------------------------------
# Done with initialization
#----------------------------------------------------------

cd ${layoutdir}

#---------------------------------------------------
# Generate GDS from the magic file.  This time,
# we do not read the LEF file, so that we pick
# up the vendor GDS database for the standard
# cells.
#
# Use magic version 8.0 to make nice labels
#---------------------------------------------------

${bindir}/magic -dnull -noconsole -T ${techpath} <<EOF
drc off
box 0 0 0 0
snap int
gds readonly true
gds rescale false
gds read ${gdspath}
def read ${rootname}
gds write ${rootname}
quit -noprompt
EOF

#------------------------------------------------------------
# Done!
#------------------------------------------------------------
