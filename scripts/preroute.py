#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from sys import path
from shutil import move

deforigx = 0
deforigy = 0
defw = 1000
defh = 1000
offsetx = 0
offsety = 0

pin_positions = {}
routed_strings = {}
netlist = []
new_metal_strings = {}

def preroute(top):
	with open(top+".def", "r") as fdef:
		for line in fdef:
			line = line.split()
			if(len(line)>1):
				if(line[0]=="DIEAREA"):
					deforigx = float(line[2])
					deforigy = float(line[3])
					defw = float(line[6])
					defh = float(line[7])

	with open(top+".preroute", "r") as pr:
		for line in pr:
			line = line.split()
			if(len(line)>1):
				key = line[0]
				if (key=="padframe"):
					origx = float(line[1])
					origy = float(line[2])
					offsetx = deforigx-origx
					offsety = deforigy-origy
				elif (key=="position"):
					pin = line[1]
					x = float(line[2])
					y = float(line[3])
					#x = x-offsetx;
					#y = y+offsety;
					point = (x,y)
					pin_positions[pin] = point
				elif (key=="net"):
					netname = line[1]
					if netname not in netlist:
						routed_strings[netname]=""
						new_metal_strings[netname]=""
						netlist.append(netname)
					routed_strings[netname]=routed_strings[netname]+" ( "+str(pin_positions[line[2]][0])+" "+str(pin_positions[line[2]][1])+" ) "
					new_metal_strings[netname]=new_metal_strings[netname]+"NEW metal1 "
					new_metal_strings[netname]=new_metal_strings[netname]+"( "+str(pin_positions[line[2]][0])+" "+str(pin_positions[line[2]][1])+" )"
					for pi in range(3,len(line)):
						new_metal_strings[netname]=new_metal_strings[netname]+" ( "+str(pin_positions[line[pi]][0])+" "+str(pin_positions[line[pi]][1])+" ) "
						#new_metal_strings[netname]=new_metal_strings[netname]+" ( * "+str(pin_positions[line[pi]][1])+" ) "
						#new_metal_strings[netname]=new_metal_strings[netname]+" ( "+str(pin_positions[line[pi]][0])+" * ) "
					new_metal_strings[netname]=new_metal_strings[netname]+"\n"
	print netlist

	net_mode = False
	net_started = False
	tmpdef = open(top+".tmp.def", "w")
	with open(top+".def", "r") as fdef:
		for line in fdef:
			sline = line.split()
			if(len(sline)>0):
				if(net_mode):
					if(net_started):
						if(sline[len(sline)-1]==";"):
							net_started = False;
							line = line.replace(";","")
							line = line + "+ ROUTED metal1"+routed_strings[netname]
							line = line + "\n"+new_metal_strings[netname]
							line = line + " ; \n"
					elif (sline[0]=="-"):
						netname = sline[1]
						if netname in netlist:
							net_started = True
					elif (sline[0]=="END"):
						net_mode = False
				elif (sline[0]=="NETS"):
					net_mode = True
			tmpdef.write(line)
	tmpdef.close()

	move(top+".tmp.def",top+".def")
