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

def place():
	chdir(project_settings.getLayoutDir())
	copyfile(project_settings.getParametersFile(),project_settings.getTopLevel()+".par")
	command = []
	command.append(settings.getGrayWolf())
	command.append("-n")
	command.append(project_settings.getTopLevel())
	graywolf = Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

	for line in iter(graywolf.stdout.readline, ''):
		print line
	graywolf.stdout.close()
