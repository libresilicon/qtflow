#!/usr/bin/python
# -*- coding: utf-8 -*-
import re
import sys

def main(inf,outf):
	pin_name = ""
	pin_use=""
	macro_name=""
	pin_mode = False
	macro_mode = False
	first_pin = True

	print "In: "+inf

	print "Out: "+outf

	file = open(inf,"r")
	outfile = open(outf,"w")

	outfile.write("EESchema-LIBRARY Version 2.3\n")
	outfile.write("#encoding utf-8\n")

	for line in file:
		if re.search("MACRO" , line):
			macro_mode = True
			first_pin = True
			idx = 0
			has_pins = False
			output_pos = -100
			input_pos = -100
			l = line.split()
			macro_name = l[1]
			outl = "DEF "+macro_name+" U 0 30 Y Y 1 F N\n"
			outl += "F0 \"U\" 0 50 60 H V C CNN\n"
			outl += "F1 \""+macro_name+"\" 0 -50 60 H V C CNN\n"
			outl += "F2 \"\" 0 0 60 H I C CNN\n"
			outl += "F3 \"\" 0 0 60 H I C CNN\n"

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

		if re.search("END" , line) and re.search(pin_name , line) and pin_mode:
			if(first_pin):
				outl += "DRAW\n"
				first_pin = False

			if(pin_use=="POWER"):
				outl += "X "+pin_name+" "+str(idx)+" 0 250 100 D 50 30 1 1 W\n"
			elif(pin_use=="GROUND"):
				outl += "X "+pin_name+" "+str(idx)+" 0 -250 100 U 50 30 1 1 P\n"
			elif(pin_direction=="INPUT"):
				outl += "X "+pin_name+" "+str(idx)+" -350 "+str(input_pos)+" 100 R 50 30 1 1 I\n"
				input_pos+=80
			elif(pin_direction=="OUTPUT"):
				outl += "X "+pin_name+" "+str(idx)+" 350 "+str(output_pos)+" 100 L 50 30 1 1 O\n"
				output_pos+=80
			elif(pin_direction=="INOUT"):
				outl += "X "+pin_name+" "+str(idx)+" 350 "+str(output_pos)+" 100 L 50 30 1 1 I\n"
				output_pos+=80

			pin_mode = False
			idx+=1

		if re.search("END" , line) and re.search(macro_name , line):
			if(macro_mode):
				if(has_pins):
					outl += "C 450 250 0 0 1 0 N\n"
					outl += "S 250 -150 -250 150 0 1 0 N\n"
					outl += "ENDDRAW\n"
					outl += "ENDDEF\n"
					outfile.write(outl)
				macro_mode = False
			

if len(sys.argv) != 3:
	print "Error, wrong number of arguments"
else:
	main(sys.argv[1],sys.argv[2])
