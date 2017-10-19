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

def synth():
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
	command+="proc ; opt; fsm; opt; memory; opt\n"
	command+="techmap; opt\n"

	command+="dfflibmap -liberty "+project_settings.getLibertyFile()+"\n"

	if(project_settings.hasSettingABC()):
		command+="abc -exe "+settings.getYosysABC()+" -liberty "+project_settings.getLibertyFile()+" -script +strash;scorr;ifraig;retime,{D};strash;dch,-f;map,-M,1,{D}\n"

	command+="flatten\n"
	command+="clean -purge\n"
	command+="opt\n"
	command+="clean\n"
	command+="rename -enumerate\n"

	command+="write_blif "+project_settings.getTopLevel()+".blif\n"
	command+="exit\n"

	p.stdin.write(command)
	p.stdin.close() # eof

	for line in iter(p.stdout.readline, ''):
		print line
		print '\n'
	p.stdout.close()
