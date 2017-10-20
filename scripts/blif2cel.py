#!/usr/bin/env python
# -*- coding: utf-8 -*-

def blif2cel(inf,outf):
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

