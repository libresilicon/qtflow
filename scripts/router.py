#!/usr/bin/env python
# -*- coding: utf-8 -*-

import io
import time
#import sys

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

chdir(project_settings.getLayoutDir())
command = []
command.append(settings.getQRouter())
#command.append("-f")
command.append("-c")
command.append(project_settings.getTopLevel()+".cfg")
command.append(project_settings.getTopLevel()+".def")

qrouter = Popen(command, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

for line in iter(qrouter.stdout.readline, ''):
	print line

qrouter.stdout.close()

move("counter_route.def","counter.def")
