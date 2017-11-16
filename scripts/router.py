#!/usr/bin/env python
# -*- coding: utf-8 -*-

import io
import time
import sys

import Tkinter

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

sys.path.append("/home/leviathan/qtflow/scripts")
import preroute

chdir(project_settings.getLayoutDir())

command = ["/usr/bin/tclsh"]
command.append("/home/leviathan/qtflow/scripts/place2def.tcl")
command.append(project_settings.getTopLevel())
command.append("FILL")
command.append(str(len(project_settings.getRoutingLayers())))
command.append(str(100))
#command.append(str(project_settings.getScaleUnit()))

p=Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

for line in iter(p.stdout.readline, ''):
	print line

p.stdout.close()

if not project_settings.isAsic():
	command = ["/usr/bin/tclsh"]
	command.append("/home/leviathan/qtflow/scripts/addspacers.tcl")
	command.append(project_settings.getTopLevel())
	command.append(project_settings.getStandardCellsLEF())
	command.append("FILL")

	p=Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

	for line in iter(p.stdout.readline, ''):
		print line

	p.stdout.close()

	move(project_settings.getTopLevel()+"_filled.def",project_settings.getTopLevel()+".def")


if path.isfile(project_settings.getTopLevel()+".def"):
	if project_settings.isAsic(): #preroute
		if path.isfile(project_settings.getTopLevel()+".preroute"):
			preroute.preroute(project_settings.getTopLevel())

	command = []
	command.append(settings.getQRouter())
	command.append("-f")
	command.append("-c")
	command.append(project_settings.getTopLevel()+".cfg")
	command.append(project_settings.getTopLevel()+".def")

	qrouter = Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

	for line in iter(qrouter.stdout.readline, ''):
		print line

	qrouter.stdout.close()

	if path.isfile(project_settings.getTopLevel()+"_route.def"):
		move(project_settings.getTopLevel()+"_route.def",project_settings.getTopLevel()+".def")
