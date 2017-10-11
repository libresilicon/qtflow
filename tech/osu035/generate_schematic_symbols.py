#!/usr/bin/python
# -*- coding: utf-8 -*-
import re
pin_name = ""
pin_use=""
macro_name=""
pin_mode = False
macro_mode = False
first_pin = True

file = open("osu035_stdcells.lef","r")
outfile = open("osu035_stdcells.sym","w")
for line in file:
	if re.search("MACRO" , line):
		macro_mode = True
		first_pin = True
		has_pins = False
		l = line.split()
		macro_name = l[1]
		outl = "DEF "+macro_name+" U 0 30 Y Y F N\n"
		outfile.write(outl)

	if re.search("PIN" , line):
		has_pins = True
		pin_mode = True
		pin_use = ""
		pin_direction = ""
		l = line.split()
		pin_name = l[1]

	if re.search("DIRECTION" , line):
		if(pin_mode):
			l = line.split()
			pin_direction = l[1]

	if re.search("USE" , line):
		if(pin_mode):
			l = line.split()
			pin_use = l[1]

	if re.search("END" , line) and re.search(pin_name , line):
		if(pin_mode):
			if(first_pin):
				outl = "DRAW\n"
				outfile.write(outl)
				first_pin = False

			if(pin_use=="POWER"):
				outl = "X "+pin_name+" -150 150 0 D 50 30 0 0 W P\n"
				outfile.write(outl)
			elif(pin_use=="GROUND"):
				outl = "X "+pin_name+" -150 150 0 D 50 30 0 0 W P\n"
				outfile.write(outl)
			elif(pin_direction=="INPUT"):
				outl = "X "+pin_name+" -150 150 0 D 50 30 0 0 W I\n"
				outfile.write(outl)
			elif(pin_direction=="OUTPUT"):
				outl = "X "+pin_name+" -150 150 0 D 50 30 0 0 W O\n"
				outfile.write(outl)
			elif(pin_direction=="INOUT"):
				outl = "X "+pin_name+" -150 150 0 D 50 30 0 0 W P\n"
				outfile.write(outl)

			pin_mode = False

	if re.search("END" , line) and re.search(macro_name , line):
		if(macro_mode):
			if(has_pins):
				outl = "ENDDRAW\n"
				outfile.write(outl)
			outl = "ENDDEF\n"
			outfile.write(outl)
			macro_mode = False
			

#outfile.write
