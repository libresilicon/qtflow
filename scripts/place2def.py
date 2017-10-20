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

from subprocess import PIPE
from subprocess import STDOUT
from subprocess import Popen

from shutil import move
from shutil import copyfile

from glob import glob
from re import search

#stuff for threads:
import commands
from threading import Thread
from Queue import Queue

def write_def_header(fdef):
	fdef.write("VERSION 5.6 ;")
	fdef.write("\n")
	fdef.write("NAMESCASESENSITIVE ON ;")
	fdef.write("\n")
	fdef.write("DIVIDERCHAR \"/\" ;")
	fdef.write("\n")
	fdef.write("BUSBITCHARS \"<>\" ;")
	fdef.write("\n")
	fdef.write("DESIGN "+project_settings.getTopLevel()+" ;")
	fdef.write("\n")
	fdef.write("UNITS DISTANCE MICRONS "+str(project_settings.getDistanceUnits())+" ;")
	fdef.write("\n")

def write_die_area(fdef):
	cellxbot = project_settings.getDistanceUnits()
	cellybot = project_settings.getDistanceUnits()

	fdef.write("DIEAREA ( 0 0 ) ( 1000 1000 ) ;")
	fdef.write("\n")

def write_cells(fdef):
	cell_list_string = ""
	cell_count = 0
	pl1name = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pl1")
	fpl1 = open(pl1name,"r")
	for line in fpl1:
		line = line.split()
		if(len(line)>1):
			instance_name = line[0];
			cell_name = search("(.+)_[0-9]+",instance_name).group(1)

			cell_list_string += "- "
			cell_list_string += instance_name
			cell_list_string += " "
			cell_list_string += cell_name
			cell_list_string += " + PLACED ( "
			cell_list_string += str(int(line[1])+500)
			cell_list_string += " "
			cell_list_string += str(int(line[2])+500)
			cell_list_string += " ) N;\n"
			cell_count+=1
	fpl1.close()

	fdef.write("COMPONENTS "+str(cell_count)+" ;")
	fdef.write("\n")
	fdef.write(cell_list_string)
	fdef.write("END COMPONENTS\n")

def place2def():
	pl2name = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pl2")
	pinname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".pin")
	defname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".def")
	obsname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".obs")
	infoname = path.join(project_settings.getLayoutDir(), project_settings.getTopLevel()+".info")

	fdef = open(defname,"w")

	write_def_header(fdef)
	write_die_area(fdef)
	write_cells(fdef)

	fdef.write("END DESIGN\n")

	fdef.close()

	print project_settings.getTopLevel()
