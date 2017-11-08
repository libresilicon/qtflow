#!/bin/tcsh -f
#
#------------------------------------------------------------------
# qflow.sh --- main program shell script
#------------------------------------------------------------------

# Environment variable overrides the tech type in all cases except
# when the technology is specified on the command line by -T.  If
# the environment variable is not set, the technology defaults to
# the technology that is issued with the qflow distribution.

set tech=`printenv QFLOW_TECH`
set has_tech=0
if ( $tech == "" ) then
   # But. . . check if there is already a "qflow_vars.sh".  If so,
   # parse it for "techname", and use that preferentially over the
   # default technology.
   if ( -f qflow_vars.sh ) then
      set tech=`cat qflow_vars.sh | grep techname | cut -d= -f2`
      set has_tech=1
   else
      set tech=osu035
      set has_tech=-1
   endif
endif

# Environment variable overrides the project root path in all cases
# except when the project root path is specified on the command line
# by -p.  If the environment variable does not exist, the project
# root directory is assumed to be the current working directory.

set project=`printenv QFLOW_PROJECT_ROOT`
if ( $project == "" ) then
   set project=`pwd`
endif

set modulename=""
set vsource=""

# Don't do anything unless told to on the command line
set actions=0
set dohelp=0
set doversion=0
set dosynth=0
set doplace=0
set dosta=0
set doroute=0
set dobackanno=0
set doclean=0
set dodisplay=0

while ($#argv > 0)
   switch($argv[1]:q)
      case -T:
      case --tech:
	 shift
	 set tech=$argv[1]
	 set has_tech=0
	 shift
	 breaksw
      case -p:
      case --project:
	 shift
	 set project=$argv[1]
	 shift
	 breaksw
      case -h:
      case --help:
	 set dohelp=1
	 shift
	 breaksw
      case -v:
      case --version:
	 set doversion=1
	 shift
	 breaksw
      case synth:
      case synthesize:
	 set dosynth=1
	 set actions=1
	 shift
	 breaksw
      case place:
	 set doplace=1
	 set actions=1
	 shift
	 breaksw
      case sta:
      case vesta:
	 set dosta=1
	 set actions=1
	 shift
	 breaksw
      case route:
	 set doroute=1
	 set actions=1
	 shift
	 breaksw
      case backanno:
	 set dobackanno=1
	 shift
	 breaksw
      case build:
	 set dosynth=1
	 set doplace=1
	 set doroute=1
	 set dobackanno=1
	 set actions=1
	 shift
	 breaksw
      case all:
	 set dosynth=1
	 set doplace=1
	 set dosta=1
	 set doroute=1
	 set dobackanno=1
	 set doclean=1
	 set dodisplay=1
	 set actions=1
	 shift
	 breaksw
      case clean:
      case cleanup:
	 set doclean=1
	 shift
	 breaksw
      case display:
	 set dodisplay=1
	 shift
	 breaksw
      case buffer:
	 echo "Note:  option buffer is deprecated."
	 shift
	 breaksw
      default:
	 if ($modulename != "") then
	    break
	 else
	    set modulename=$argv[1]
	    shift
	 endif
	 breaksw
   endsw
end

if ($doversion == 1 || $dohelp == 1) then
   echo "Qflow version 1.1 revision 67"
   echo ""

   if ($doversion == 1) then
      exit 0
   endif
endif

if ($dohelp == 1 || $#argv != 0) then
   echo "Usage: qflow [processes] [options] <module_name>"
   echo "Processes:  synthesize			Synthesize verilog source"
   echo "            place			Run initial placement"
   echo "            sta			Static timing analysis"
   echo "            route			Run placement and route"
   echo "            backanno			Post-route timing analysis"
   echo "            clean			Remove temporary working files"
   echo "            display			Display routed result"
   echo ""
   echo "            build			Run scripts synthesize to route"
   echo "            all			Run scripts synthesize to display"
   echo ""
   echo "Options:    -T, --tech <name>		Use technology <name>"
   echo "            -p, --project <name>	Project root directory is <name>"
   if ($dohelp == 1) then
      exit 0
   else
      exit 1
   endif
endif

echo ""
echo "--------------------------------"
echo "Qflow project setup"
echo "--------------------------------"
echo ""

if ($has_tech == 1) then
   echo "Technology set to $tech from existing qflow_vars.sh file"
else if ($has_tech == -1) then
   echo "No technology specified or found;  using default technology $tech"
else
   echo "Technology set to $tech"
endif

source /usr/share/qflow/scripts/checkdirs.sh ${tech} ${project}

set nvsrc = `ls ${sourcedir}/*.v | grep -v match | wc -l`
set nsvsrc = `ls ${sourcedir}/*.sv | wc -l`

# Verilog file root name is the same as the module name, at least until
# discovered otherwise.  If the module name is not given on the command
# line, then there must be one source file and the filename root and
# module name must match.

if ($modulename == "") then
   if ($nvsrc == 1) then
      set vsource=`ls ${sourcedir}/*.v`
      set modulename=${vsource:r}
   else if ($nsvsrc == 1) then
      set vsource=`ls ${sourcedir}/*.sv`
      set modulename=${vsource:r}
   else if ($nvsrc == 0 && $nsvsrc == 0) then
      echo "Error:  No verilog source files found in directory ${sourcedir}."
      exit 1
   else
      echo "Error:  No verilog source file or module name has been specified"
      echo "and directory ${sourcedir} contains multiple verilog files."
      echo "Please specify the source file or module name on the command line."
      exit 1
   endif
endif

if ($vsource == "") then
   # Check all .v and .sv files for one with the specified module.

   foreach file ( `ls ${sourcedir}/*.v ${sourcedir}/*.sv` )
      set mline = `cat $file | grep module | grep ${modulename} | wc -l`
      if ( $mline > 0 ) then
	 set vsource = ${file}
	 break
      endif
   end

   if ( ${vsource} == "" ) then
      echo "Error:  No verilog file in ${sourcedir} contains module ${modulename}."
      exit 1
   endif
endif

#------------------------------------------------------------------
# Source the technology initialization script
#------------------------------------------------------------------

if ( -f ${techdir}/${tech}.sh ) then
   source $techdir/${tech}.sh
else
   echo "Error:  Cannot find tech init script ${techdir}/${tech}.sh to source"
   exit 1
endif

#------------------------------------------------------------------
# Prepare the script file to run in the project directory.  We
# specify all steps of the process and comment out those that
# have not been selected.  Finally, source the script.
#------------------------------------------------------------------

set varfile=${projectpath}/qflow_vars.sh
set execfile=${projectpath}/qflow_exec.sh
set userfile=${projectpath}/project_vars.sh

#------------------------------------------------------------------
# Check if a variables file exists.  If so, note that we are
# regenerating the flow, source the file, check if the technology
# is being changed, and report if so.
#------------------------------------------------------------------

set newtech = 0
if ( -f $varfile ) then
   set techorig=`cat $varfile | grep techname= | cut -d= -f2`
   if ( "${tech}" != "${techorig}" ) then
      echo "Warning:  Project technology changed from ${techorig} to ${tech}"
      set newtech = 1
   else
      echo "Regenerating files for existing project ${modulename}"
   endif
endif

echo "#\!/bin/tcsh -f" > ${varfile}
echo "#-------------------------------------------" >> ${varfile}
echo "# qflow variables for project ${project}" >> ${varfile}
echo "#-------------------------------------------" >> ${varfile}
echo "" >> ${varfile}

echo "set projectpath=${projectpath}" >> ${varfile}
echo "set techdir=${techdir}" >> ${varfile}
echo "set sourcedir=${sourcedir}" >> ${varfile}
echo "set synthdir=${synthdir}" >> ${varfile}
echo "set layoutdir=${layoutdir}" >> ${varfile}
echo "set techname=${techname}" >> ${varfile}
echo "set scriptdir=${scriptdir}" >> ${varfile}
echo "set bindir=${bindir}" >> ${varfile}
echo "set logdir=${projectpath}/log" >> ${varfile}
echo "#-------------------------------------------" >> ${varfile}
echo "" >> ${varfile}

echo "#\!/bin/tcsh -f" > ${execfile}
echo "#-------------------------------------------" >> ${execfile}
echo "# qflow exec script for project ${project}" >> ${execfile}
echo "#-------------------------------------------" >> ${execfile}
echo "" >> ${execfile}

#-----------------------------------------------------
# The file "project_vars.sh" will ONLY be written if
# one does not already exist, and then it will be an
# empty file with a few pointers to values that can
# be set by the user.
#-----------------------------------------------------

if ( ! -f ${userfile} ) then
   echo "#\!/bin/tcsh -f" > ${userfile}
   echo "#------------------------------------------------------------" >> ${userfile}
   echo "# project variables for project ${project}" >> ${userfile}
   echo "#------------------------------------------------------------" >> ${userfile}
   echo "" >> ${userfile}
   echo "# Synthesis command options:" >> ${userfile}
   echo "# -------------------------------------------" >> ${userfile}
   echo "# set yosys_options = " >> ${userfile}
   echo "# set yosys_script = " >> ${userfile}
   echo "# set yosys_debug = " >> ${userfile}
   echo "# set abc_script = " >> ${userfile}
   echo "# set nobuffers = " >> ${userfile}
   echo "# set nofanout = " >> ${userfile}
   echo "# set fanout_options = " >> ${userfile}
   echo "" >> ${userfile}
   echo "# Placement command options:" >> ${userfile}
   echo "# -------------------------------------------" >> ${userfile}
   echo "# set initial_density = " >> ${userfile}
   echo "# set graywolf_options = " >> ${userfile}
   echo '# set addspacers_options = "-stripe 5 200 PG -nostretch"' >> ${userfile}
   echo "" >> ${userfile}
   echo "# Router command options:" >> ${userfile}
   echo "# -------------------------------------------" >> ${userfile}
   echo "# set route_show = " >> ${userfile}
   echo "# set route_layers = " >> ${userfile}
   echo "# set via_stacks = " >> ${userfile}
   echo "# set qrouter_options = " >> ${userfile}
   echo ""
   echo "" >> ${userfile}
   echo "# Minimum operating period of the clock (in ps)" >> ${userfile}
   echo '# set vesta_options = "--summary reports --long --period 1E5"' >> ${userfile}
   echo "" >> ${userfile}
   echo "#------------------------------------------------------------" >> ${userfile}
   echo "" >> ${userfile}
endif

if ($dosynth == 0) then
   echo -n "# " >> ${execfile}
endif
if ( ${vsource} == "" ) then
   echo "${scriptdir}/synthesize.sh ${projectpath} ${modulename} || exit 1" >> ${execfile}
else
   echo "${scriptdir}/synthesize.sh ${projectpath} ${modulename} ${vsource} || exit 1" >> ${execfile}
endif

if ($doplace == 0) then
   echo -n "# " >> ${execfile}
endif
# Use -d because the user may decide not to run fanout buffering,
# and the files generated by place2def.tcl are required for routing.
echo "${scriptdir}/placement.sh -d ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ($dosta == 0) then
   echo -n "# " >> ${execfile}
endif
echo "${scriptdir}/vesta.sh ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ($doroute == 0) then
   echo -n "# " >> ${execfile}
endif
echo "${scriptdir}/router.sh ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ($dobackanno == 0) then
   echo -n "# " >> ${execfile}
endif
echo "${scriptdir}/vesta.sh -d ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ($doclean == 0) then
   echo -n "# " >> ${execfile}
endif
echo "${scriptdir}/cleanup.sh ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ($dodisplay == 0) then
   echo -n "# " >> ${execfile}
endif
echo "${scriptdir}/display.sh ${projectpath} ${modulename} || exit 1" >> ${execfile}

if ( $actions == 0 ) then
   echo "No actions specified on command line;"
   echo "creating qflow script file ${execfile} only."
   echo "Uncomment lines in this file and source the file to run the flow."
endif

chmod u+x ${execfile}

# Drop the magic startup file into the layout directory if it does not exist
# If it exists but the technology directory has a newer file, or if we are
# changing technologies, then copy the old file to a backup and create a new
# one.

if (-d ${layoutdir}) then
   if (!(-f ${layoutdir}/.magicrc)) then
      cp ${techdir}/${magicrc} ${layoutdir}/.magicrc
   else if ( $newtech == 1 ) then
      echo "Technology changed:  Old .magicrc file moved to .magicrc.orig"
      cp ${layoutdir}/.magicrc ${layoutdir}/.magicrc.orig
      cp ${techdir}/${magicrc} ${layoutdir}/.magicrc
   else if ( -M ${techdir}/${magicrc} > -M ${layoutdir}/.magicrc ) then
      echo -n "Technology .magicrc file has been updated.  "
      echo "Old .magicrc file moved to .magicrc.orig"
      cp ${layoutdir}/.magicrc ${layoutdir}/.magicrc.orig
      cp ${techdir}/${magicrc} ${layoutdir}/.magicrc
   endif
endif

# Drop the GrayWolf parameter file into the layout directory if it does not
# exist.  Like the above, check if the techdir has a newer version, or if we
# are changing technologies.

if (-d ${layoutdir}) then
   if (!(-f ${layoutdir}/${modulename}.par)) then
      cp ${techdir}/${techname}.par ${layoutdir}/${modulename}.par
   else if ( $newtech == 1 ) then
      echo "Technology changed:  Old .par file moved to .par.orig"
      cp ${layoutdir}/${modulename}.par ${layoutdir}/${modulename}.par.orig
      cp ${techdir}/${techname}.par ${layoutdir}/${modulename}.par
   else if ( -M ${techdir}/${techname}.par > -M ${layoutdir}/${modulename}.par ) then
      echo "Technology .par file has been updated.  Old .par file moved to .par.orig"
      cp ${layoutdir}/${modulename}.par ${layoutdir}/${modulename}.par.orig
      cp ${techdir}/${techname}.par ${layoutdir}/${modulename}.par
   endif
endif


# Execute the script file to run any command that has not been commented out
exec ${execfile}

exit 0
