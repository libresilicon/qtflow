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

def blif2sym():
	inf = path.join(project_settings.getSynthesisDir(), project_settings.getTopLevel()+".blif")
	outf = path.join(project_settings.getSourceDir(), project_settings.getTopLevel()+".sym")

	print "Opening file "+inf+" for reading"
	print "Opening file "+outf+" for writing"

	outfp = open(outf,"w")
	infp = open(inf,"r")

	outl = "EESchema-LIBRARY Version 2.3\n"
	outl += "\n"
	outl += "#encoding utf-8"
	outl += "\n"

	outl += "DEF "
	outl += project_settings.getTopLevel()
	outl += " U 0 30 Y Y 1 F N"
	outl += "\n"

	outl += "F0 \"U\" 0 50 60 H V C CNN"
	outl += "\n"

	outl += "F1 \""
	outl += project_settings.getTopLevel()
	outl += "\" 0 -50 60 H V C CNN"
	outl += "\n"

	outl += "DRAW"
	outl += "\n"

	pin_count = 0
	for line in infp:
		line = line.split()
		if(len(line)>1):
		    if(line[0]==".inputs"):
			for i in range(1,len(line)):
			    outl += "X "
			    outl += line[i]
			    outl += " " + str(pin_count)


#	X A 0 -350 -100 100 R 50 30 1 1 I
#	X B 1 -350 -20 100 R 50 30 1 1 I

#	X Y 3 350 -100 100 L 50 30 1 1 O


	outl += "C 450 250 0 0 1 0 N"
	outl += "\n"
	outl += "S 250 -150 -250 150 0 1 0 N"
	outl += "\n"

	outl += "ENDDRAW"
	outl += "\n"
	outl += "ENDDEF"
	outl += "\n"
