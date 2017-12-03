#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from sys import path
from shutil import move

pfx1 = 0
pfy1 = 0
pfx2 = 0
pfy2 = 0
corex1 = 0
corey1 = 0
corex2 = 0
corey2 = 0
defpfx1 = 0
defpfy1 = 0
defpfx2 = 0
defpfy2 = 0
defw = 1000
defh = 1000
offsetx = 0
offsety = 0

part_positions = {}
pin_positions = {}
pin_nets = {}
routed_strings = {}
netlist = []
general_netlist = []
new_metal_strings = {}
reading_components = False
signal_types = {}
signal_layers = {}
cell_component_mapping = {}
cell_dimensions = {}
cell_orientation_mapping = {}
routed_nets = []
pad_cells = []
pad_banks = {}
pad_banks_offsets = {}
pins_section = {}

def read_preroute_file(file):
	global pfx1
	global pfy1
	global pfx2
	global pfy2
	global corex1
	global corey1
	global corex2
	global corey2
	with open(file, "r") as pr:
		for line in pr:
			line = line.split()
			if(len(line)>1):
				key = line[0]
				if (key=="padframe"):
					pfx1 = float(line[1])
					pfx2 = float(line[2])
					pfy1 = float(line[3])
					pfy2 = float(line[4])
				elif (key=="core"):
					corex1 = float(line[1])
					corex2 = float(line[2])
					corey1 = float(line[3])
					corey2 = float(line[4])
				elif (key=="position"):
					try:
						pp = pin_positions[component]
					except:
						pp = {}
					component = line[1]
					pin = line[2]
					x = float(line[3])
					y = float(line[4])
					w = float(line[5])
					h = float(line[6])
					point = (x,y,w,h)
					pp[pin] = point
					pin_positions[component]=pp
				elif (key=="net"):
					netlist.append(line[1])
				elif (key=="use"):
					signal_name = line[1]
					signal_type = line[2]
					signal_types[signal_name]=signal_type
				elif (key=="layer"):
					signal_name = line[1]
					signal_layer = line[2]
					signal_layers[signal_name]=signal_layer
				elif (key=="dimension"):
					cell_name = line[1]
					w = float(line[2])
					h = float(line[3])
					dim = (w,h)
					cell_dimensions[cell_name] = dim
				elif (key=="padcell"):
					cell_name = line[1]
					pad_cells.append(cell_name)

def read_def_info(file):
	global defpfx1
	global defpfy1
	global defpfx2
	global defpfy2
	global part_positions
	components_mode = False
	nets_mode = False
	with open(file, "r") as fdef:
		for line in fdef:
			sline = line.split()
			if(len(sline)>0):
				key = sline[0]
				if(components_mode):
					if(key=="-"):
						instance_name = sline[1]
						cell_name = sline[2]
						cell_component_mapping[instance_name]=cell_name
						if(sline[4]=="PLACED"):
							x = float(sline[6])
							y = float(sline[7])
							part_positions[instance_name]=(x,y)
							cell_orientation_mapping[instance_name]=sline[9]
					elif(key=="END"):
						components_mode = False
				elif(nets_mode):
					if(key=="-"):
						net_name = sline[1]
						if not (net_name in general_netlist):
							general_netlist.append(net_name)
					elif(key=="("):
						instance_name = sline[1]
						try:
							pp = pin_nets[instance_name]
						except:
							pp = {}
						pin_name = sline[2]
						pp[pin_name] = net_name
						pin_nets[instance_name] = pp
					elif(key=="END"):
						nets_mode = False
				elif(key=="DIEAREA"):
					defpfx1 = float(sline[2])
					defpfy1 = float(sline[3])
					defpfx2 = float(sline[6])
					defpfy2 = float(sline[7])
				elif(key=="COMPONENTS"):
					components_mode = True
				elif(key=="NETS"):
					nets_mode = True

def cleanup_def(file):
	net_section = False
	write_out = False
	tmpdef = open(file+".tmp", "w")
	with open(file, "r") as fdef:
		for line in fdef:
			sline = line.split()
			if(len(sline)>0):
				key = sline[0]
				if(net_section):
					if(key=="END"):
						net_section = False
						tmpdef.write("\n")
						tmpdef.write(line)
					elif(write_out):
						for net in general_netlist:
							tmpdef.write("\n- "+net)
							for part in pin_nets:
								for pin in pin_nets[part]:
									if(pin_nets[part][pin]==net):
										tmpdef.write("\n  ( "+part+" "+pin+" )")
							tmpdef.write("\n;\n")
							write_out = False

				elif(key=="NETS"):
					net_section = True
					write_out = True
					tmpdef.write("NETS "+str(len(general_netlist))+" ; ")
				else:
					tmpdef.write(line)
	tmpdef.close()
	move(file+".tmp",file)


def set_dimensions_from_def():
	global corex1
	global corey1
	global corex2
	global corey2

	corew = corex2-corex1
	coreh = corey2-corey1

	firsValue = True
	for instance_name in part_positions:
		if not (cell_component_mapping[instance_name] in pad_cells):
			if(firsValue):
				corex1 = part_positions[instance_name][0]
				corey1 = part_positions[instance_name][1]
				firsValue = False
			else:
				if(corex1>part_positions[instance_name][0]):
					corex1 = part_positions[instance_name][0]
				if(corey1>part_positions[instance_name][1]):
					corey1 = part_positions[instance_name][1]
	corex2 = corex1+corew
	corey2 = corey1+coreh

def fill_pad_banks():
	global pad_banks
	global bank_pad_wiring
	global pad_banks_offsets
	for part in pin_nets:
		if (part in cell_component_mapping) and (part in part_positions):
			cell_name = cell_component_mapping[part]
			if cell_name in pin_positions:
				for pin in pin_nets[part]:
					orient = cell_orientation_mapping[part]
					try:
						l = pad_banks[orient]
					except:
						l = []
					l.append(part)
					pad_banks[orient]=l


def get_pad_bank_dimensions():
	global corex1
	global corey1
	global corex2
	global corey2
	global bank_pad_wiring
	global pad_banks_offsets
	first = {}
	for orient in pad_banks:
		first[orient]=True
	for orient in pad_banks:
		for part in pad_banks[orient]:
			cell_name = cell_component_mapping[part]
			for pin in pin_positions[cell_name]:
				if pin in pin_nets[part]:
					net = pin_nets[part][pin]
					points = []
					net = pin_nets[part][pin]
					wx = part_positions[part][0]
					wy = part_positions[part][1]
					if(orient=="N"):
						wx = wx+pin_positions[cell_name][pin][0]
						wx = wx+pin_positions[cell_name][pin][2]/2
						wy = wy+pin_positions[cell_name][pin][1]
						wy = wy+pin_positions[cell_name][pin][3]/2
					if(orient=="S"):
						wx = wx+cell_dimensions[cell_name][0]
						wx = wx-pin_positions[cell_name][pin][0]
						wx = wx-pin_positions[cell_name][pin][2]/2
						wy = wy+cell_dimensions[cell_name][1]
						wy = wy-pin_positions[cell_name][pin][1]
						wy = wy-pin_positions[cell_name][pin][3]/2
					if(orient=="W"):
						wx = wx+cell_dimensions[cell_name][1]
						wx = wx-pin_positions[cell_name][pin][1]
						wx = wx-pin_positions[cell_name][pin][3]/2
						wy = wy+pin_positions[cell_name][pin][0]
						wy = wy+pin_positions[cell_name][pin][2]/2
					if(orient=="E"):
						wx = wx+pin_positions[cell_name][pin][1]
						wx = wx+pin_positions[cell_name][pin][3]/2
						wy = wy+cell_dimensions[cell_name][0]
						wy = wy-pin_positions[cell_name][pin][0]
						wy = wy-pin_positions[cell_name][pin][2]/2

					if(first[orient]):
						first[orient]=False
						x1 = wx
						y1 = wy
						x2 = wx
						y2 = wy
					else:
						x1 = pad_banks_offsets[orient][0]
						y1 = pad_banks_offsets[orient][1]
						x2 = pad_banks_offsets[orient][2]
						y2 = pad_banks_offsets[orient][3]
						if(x1>wx):
							x1 = wx
						if(y1>wy):
							y1 = wy
						if(x2<wx):
							x2 = wx
						if(y2<wy):
							y2 = wy

					pad_banks_offsets[orient]=(x1,y1,x2,y2)


net_ring_dimensions = {}
power_ring_wiring = {}
def setup_power_rings():
	global corex1
	global corey1
	global corex2
	global corey2
	global net_ring_dimensions

	routed_power_nets = []

	ring_count = 1
	wire_width = 20
	ring_wires = 20
	ring_width = ring_wires*wire_width
	ring_offset = ring_width*2

	for orient in pad_banks:
		for part in pad_banks[orient]:
			#print part
			cell_name = cell_component_mapping[part]
			for pin in pin_positions[cell_name]:
				if pin in pin_nets[part]:
					net = pin_nets[part][pin]
					if not (net in routed_power_nets):
						routed_power_nets.append(net)
						if((signal_types[net]=="POWER")or(signal_types[net]=="GROUND")):
							try:
								point_group = power_ring_wiring[net]
							except:
								point_group = []

							wx1 = corex1-ring_count*ring_offset
							wy1 = corey1-ring_count*ring_offset
							for i in range(1,ring_wires):
								points = []
								wx1 = corex1-ring_count*ring_offset-(i+ring_count)*wire_width
								wy1 = corey1-ring_count*ring_offset-(i+ring_count)*wire_width
								wx2 = corex2+ring_count*ring_offset+(i+ring_count)*wire_width
								wy2 = corey2+ring_count*ring_offset+(i+ring_count)*wire_width
								point = (wx1,wy1)
								points.append(point)
								point = (wx1,wy2)
								points.append(point)
								point = (wx2,wy2)
								points.append(point)
								point = (wx2,wy1)
								points.append(point)
								point = (wx1,wy1)
								points.append(point)
								square = (wx1,wy1,wx2,wy2)
								net_ring_dimensions[net] = square
								point_group.append(points)

							ring_count=ring_count+1
							power_ring_wiring[net] = point_group

bank_pad_wiring = {}
def setup_wiring():
	global corex1
	global corey1
	global corex2
	global corey2
	global bank_pad_wiring
	global pad_banks_offsets
	global net_ring_dimensions
	global pins_section
	dfactor=10000
	#padoffset=4000
	padoffset=400
	get_pad_bank_dimensions()
	for orient in pad_banks:
		for part in pad_banks[orient]:
			cell_name = cell_component_mapping[part]
			for pin in pin_positions[cell_name]:
				if pin in pin_nets[part]:
					net = pin_nets[part][pin]

					try:
						point_group = bank_pad_wiring[net]
					except:
						point_group = []

					points = []
					net = pin_nets[part][pin]
					wx = part_positions[part][0]
					wy = part_positions[part][1]
					pbw = abs(pad_banks_offsets[orient][2]-pad_banks_offsets[orient][0])
					pbh = abs(pad_banks_offsets[orient][3]-pad_banks_offsets[orient][1])
					corew = abs(corex2-corex1)
					coreh = abs(corey2-corey1)
					if(orient=="N"):
						wx = wx+pin_positions[cell_name][pin][0]
						wx = wx+pin_positions[cell_name][pin][2]/2
						wy = wy+pin_positions[cell_name][pin][1]
						wy = wy+pin_positions[cell_name][pin][3]/2
						point = (wx,wy)
						points.append(point)
						dy = dfactor*abs((pbw/2)-abs((wx-pad_banks_offsets[orient][0])))/pbw
						wy = wy-dy
						point = (wx,wy)
						points.append(point)
						wx = (corew*(wx-pad_banks_offsets[orient][0])/pbw)+corex1
						point = (wx,wy)
						points.append(point)
						wy = corey2+padoffset
						if net in net_ring_dimensions:
							wy = net_ring_dimensions[net][3]
						point = (wx,wy)
						points.append(point)
						pins_section[net]=point
					if(orient=="S"):
						wx = wx+cell_dimensions[cell_name][0]
						wx = wx-pin_positions[cell_name][pin][0]
						wx = wx-pin_positions[cell_name][pin][2]/2
						wy = wy+cell_dimensions[cell_name][1]
						wy = wy-pin_positions[cell_name][pin][1]
						wy = wy-pin_positions[cell_name][pin][3]/2
						point = (wx,wy)
						points.append(point)
						dy = dfactor*abs((pbw/2)-abs((wx-pad_banks_offsets[orient][0])))/pbw
						wy = wy+dy
						point = (wx,wy)
						points.append(point)
						wx = (corew*(wx-pad_banks_offsets[orient][0])/pbw)+corex1
						point = (wx,wy)
						points.append(point)
						wy = corey1-padoffset
						if net in net_ring_dimensions:
							wy = net_ring_dimensions[net][1]
						point = (wx,wy)
						points.append(point)
						pins_section[net]=point
					if(orient=="W"):
						wx = wx+cell_dimensions[cell_name][1]
						wx = wx-pin_positions[cell_name][pin][1]
						wx = wx-pin_positions[cell_name][pin][3]/2
						wy = wy+pin_positions[cell_name][pin][0]
						wy = wy+pin_positions[cell_name][pin][2]/2
						point = (wx,wy)
						points.append(point)
						dx = dfactor*abs((pbh/2)-abs((wy-pad_banks_offsets[orient][1])))/pbh
						wx = wx+dx
						point = (wx,wy)
						points.append(point)
						wy = (coreh*abs(wy-pad_banks_offsets[orient][1])/pbh)+corey1
						point = (wx,wy)
						points.append(point)
						wx = corex1-padoffset
						if net in net_ring_dimensions:
							wx = net_ring_dimensions[net][0]
						point = (wx,wy)
						points.append(point)
						pins_section[net]=point
					if(orient=="E"):
						wx = wx+pin_positions[cell_name][pin][1]
						wx = wx+pin_positions[cell_name][pin][3]/2
						wy = wy+cell_dimensions[cell_name][0]
						wy = wy-pin_positions[cell_name][pin][0]
						wy = wy-pin_positions[cell_name][pin][2]/2
						point = (wx,wy)
						points.append(point)
						dx = dfactor*abs((pbh/2)-abs((wy-pad_banks_offsets[orient][1])))/pbh
						wx = wx-dx
						point = (wx,wy)
						points.append(point)
						wy = (coreh*abs(wy-pad_banks_offsets[orient][1])/pbh)+corey1
						point = (wx,wy)
						points.append(point)
						wx = corex2+padoffset
						if net in net_ring_dimensions:
							wx = net_ring_dimensions[net][2]
						point = (wx,wy)
						points.append(point)
						pins_section[net]=point
					point_group.append(points)
					bank_pad_wiring[net] = point_group



def preroute(top):
	global corex1
	global corey1
	global corex2
	global corey2
	global bank_pad_wiring
	global pins_section

	read_preroute_file(top+".preroute")
	read_def_info(top+".def")

	xoffset = defpfx1-pfx1
	yoffset = defpfy1-pfy1

	set_dimensions_from_def()

	net_section = False
	net_begone = False

	# cleaning up the DEF file (declare each net only once)
	cleanup_def(top+".def")

	metal_layer = "metal1"
	net_section = False
	net_begone = False
	pins_started = False

	fill_pad_banks()
	setup_power_rings()
	setup_wiring()

	tmpdef = open(top+".tmp.def", "w")
	with open(top+".def", "r") as fdef:
		for line in fdef:
			sline = line.split()
			if(len(sline)>0):
				key = sline[0]
				if(net_section):
					if(key=="END"):
						net_section = False
					if(net_begone):
						if(sline[len(sline)-1]==";") :
							line = line.replace(";","")
							line = line + "+ USE "+signal_types[net]+"\n"
							try:
								pointslist = bank_pad_wiring[net]
							except:
								pointslist = []

							if(len(pointslist)>0):
								points = pointslist[0]
								#line = line + "+ FIXED\n"
								line = line + "+ ROUTED "+metal_layer
								for point in points:
									line = line + " ( "+str(point[0])+" "+str(point[1])+" )"
									#line = line + " via"
								#line = line + " M2_M1"

							if(len(pointslist)>1):
								for i in range(1,len(pointslist)):
									points = pointslist[i]
									if(len(points)>0):
										line = line + "\n  NEW "+metal_layer
										for point in points:
											line = line + " ( "+str(point[0])+" "+str(point[1])+" )"
											#line = line + " via"
										#line = line + " M2_M1"

							try:
								pointslist = power_ring_wiring[net]
							except:
								pointslist = []

							if(len(pointslist)>0):
								for points in pointslist:
									if(len(points)>0):
										line = line + "\n  NEW "+metal_layer
										for point in points:
											line = line + " ( "+str(point[0])+" "+str(point[1])+" )"
											#line = line + " via"
										#line = line + " M2_M1"

							line = line + " ;\n"
							net_begone = False
					elif (key=="-"):
						net = sline[1]
						try:
							metal_layer = signal_layers[net]
						except:
							metal_layer = "metal1"

						if net in netlist:
							net_begone = True
				elif(key=="NETS"):
					net_section = True
				elif(key=="PINS"):
					line = "PINS "+str(len(pins_section))+" ;"
					for net in pins_section:
						line = line+"\n"
						line = line+"- "+net+" + NET "+net
						line = line+"\n"
						line = line+"  + LAYER metal3 ( -50 -50 ) ( 50 50 )"
						line = line+"\n"
						line = line+"+ PLACED ( "+str(pins_section[net][0])+" "+str(pins_section[net][1])+" ) N ;"
					line = line+"\n"
			tmpdef.write(line)
	tmpdef.close()

	move(top+".tmp.def",top+".def")
