#!/usr/bin/env python
# -*- coding: utf-8 -*-

def blif2cel(inf,outf):
	print "Opening file "+inf+" for reading"
	print "Opening file "+outf+" for writing"

	cell_count = 0
	instance_counter = {}

	infp = open(inf,"r")
	for line in infp:
		line = line.split()
		if len(line) > 1:
			if(line[0]==".subckt"):
				if not instance_counter.has_key(line[1]):
					instance_counter[line[1]] = 1

				print "cell "+str(cell_count)+" "+line[1]+"_"+str(instance_counter[line[1]])
				cell_count+=1
				instance_counter[line[1]]+=1
