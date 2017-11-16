#!/usr/bin/env python
# -*- coding: utf-8 -*-

import io
import time
import sys

from os import path
from os import popen
from os import listdir
from os import remove
from os import chdir
from os import path

from re import search

from subprocess import PIPE
from subprocess import STDOUT
from subprocess import Popen

from shutil import move
from glob import glob

chdir(project_settings.getSynthesisDir())
p=Popen(settings.getYosys(), stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)
command=""
command+="read_liberty -lib -ignore_miss_dir -setattr blackbox "+project_settings.getLibertyFile()+"\n"
command+="read_verilog "

for f in project_settings.getSearchDirectories():
	command+="-I"
	command+=f
	command+=" "

command+=project_settings.getTopLevelFile()+"\n"

command+="synth -top "+project_settings.getTopLevel()+"\n"
command+="hierarchy -check -top "+project_settings.getTopLevel()+"\n"
command+="proc;\n"
command+="flatten;\n"

command+="opt;\n"
command+="fsm;\n"
command+="opt;\n"
command+="memory;\n"
command+="opt\n"
command+="techmap;\n"
command+="opt\n"

command+="dfflibmap -liberty "+project_settings.getLibertyFile()+"\n"

#if(project_settings.hasSettingABC()):
command+="abc -exe "+settings.getYosysABC()+" -liberty "+project_settings.getLibertyFile()+" -script +strash;scorr;ifraig;retime,{D};strash;dch,-f;map,-M,1,{D}\n"

command+="clean -purge\n"
command+="opt\n"
command+="clean\n"
command+="rename -enumerate\n"
command+="write_blif "+project_settings.getTopLevel()+"_mapped.blif\n"
command+="exit\n"

p.stdin.write(command)
p.stdin.close() # eof

for line in iter(p.stdout.readline, ''):
	print line

p.stdout.close()

command = ["/usr/bin/tclsh"]
command.append("/home/leviathan/qtflow/scripts/ypostproc.tcl")
command.append(project_settings.getTopLevel()+"_mapped.blif")
command.append(project_settings.getTopLevel())
command.append("/usr/share/qflow/tech/osu018/osu018.sh")

p=Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

for line in iter(p.stdout.readline, ''):
	print line

p.stdout.close()

command = ["/usr/bin/tclsh"]
command.append("/home/leviathan/qtflow/scripts/ybuffer.tcl")
command.append(project_settings.getTopLevel()+"_mapped_tmp.blif")
command.append(project_settings.getTopLevel()+".blif")
command.append("/usr/share/qflow/tech/osu018/osu018.sh")

p=Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

for line in iter(p.stdout.readline, ''):
	print line

p.stdout.close()


command = ["/usr/bin/tclsh"]
command.append("/home/leviathan/qtflow/scripts/blif2cel.tcl")
command.append("--lef")
command.append(project_settings.getStandardCellsLEF())
command.append("--blif")
command.append(project_settings.getTopLevel()+".blif")
command.append("--cel")
command.append(path.join(project_settings.getLayoutDir(),project_settings.getTopLevel()+".cel"))
command.append("--units")
#command.append(str(project_settings.getScaleUnit()))
command.append(str(100))
command.append("--pad-width")
command.append("100")
command.append("--pad-height")
command.append("100")
if project_settings.isAsic():
	print "Is ASIC"
	command.append("--no-pads") #we will provide our own padframe mapping

p=Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

for line in iter(p.stdout.readline, ''):
	print line

p.stdout.close()

