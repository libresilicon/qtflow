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

def blif2cel():
	inf = path.join(project_settings.getSynthesisDir(), project_settings.getTopLevel()+".blif")
	outf = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".cel")

	print "Opening file "+inf+" for reading"
	print "Opening file "+outf+" for writing"

	cell_count = 0
	instance_counter = {}

	outfp = open(outf,"w")
	infp = open(inf,"r")
	for line in infp:
		line = line.split()
		if len(line) > 1:
			if(line[0]==".subckt"):
				if not instance_counter.has_key(line[1]):
					instance_counter[line[1]] = 1

				ol = "\ncell "+str(cell_count)+" "+line[1]+"_"+str(instance_counter[line[1]])
				ol += "\nleft "
				ol += str(project_settings.getMacroValue(line[1],"left"))
				ol += " right "
				ol += str(project_settings.getMacroValue(line[1],"right"))
				ol += " bottom "
				ol += str(project_settings.getMacroValue(line[1],"bottom"))
				ol += " top "
				ol += str(project_settings.getMacroValue(line[1],"top"))

				for idx in range(2,len(line)):
					pin_str = line[idx]
					pin_inf = pin_str.split('=')
					if(len(pin_inf)>1):
						ol += "\npin name "
						ol += pin_inf[0]
						ol += " signal "
						ol += pin_inf[1]
						ol += " layer 1 "
						ol += str(project_settings.getMacroPinValue(line[1],pin_inf[0],"xp"))
						ol += " "
						ol += str(project_settings.getMacroPinValue(line[1],pin_inf[0],"yp"))

				outfp.write(ol)

				cell_count+=1
				instance_counter[line[1]]+=1
	infp.close()
	outfp.close()

