import io
import time
import sys

from os import path
from os import popen
from os import listdir
from os import remove
from os import chdir
from os import path

from re import search

from subprocess import PIPE
from subprocess import STDOUT
from subprocess import Popen

from shutil import move
from glob import glob

def add_before_end(filepath, lines):
	f = open(filepath,'r')
	n = open(filepath+".temp",'w')
	initial_found = False
	end_token = True

	for line in f:
		if search("initial", line):
			initial_found = True
		else:
			if(initial_found and end_token):
				if search("end", line):
					n.write(lines) 
					end_token = False

		n.write(line)

	f.close()
	n.close()

	remove(filepath)
	move(filepath+".temp",filepath)

def add_initial(filepath):
	f = open(filepath,'r')
	n = open(filepath+".temp",'w')
	
	start_found = False
	done = False
	for line in f:
		if search(project_settings.getTestBench(), line):
			start_found = True
		else:
			if start_found and search("endmodule", line):
				if not done:
					n.write("\ninitial begin\nend\n")
					done = True
		n.write(line)

	f.close()
	n.close()

	remove(filepath)
	move(filepath+".temp",filepath)

def check_initial(filepath):
	initial_set = False

	f = open(filepath, "r")
	for line in f:
		if search("initial", line):
			initial_set = True
			break

	f.close()

	if not initial_set:
		add_initial(filepath)

def add_dumpvars(filepath):
	add_before_end(filepath, "\n$dumpvars(0,"+project_settings.getTestBench()+");\n");

def add_finish(filepath):
	add_before_end(filepath, "\n# 1501 $finish;\n");

def add_dumpfile(filepath):
	add_before_end(filepath, "\n$dumpfile(\""+project_settings.getVCDFile()+"\");\n");

def check_and_fix(filepath):
	dumpvars_name = project_settings.getVCDFile()
	f = open(filepath, "r")
	dumpvars_set = False
	dumpfile_set =  False
	finish_set = False

	for line in f:
		if search("dumpvars", line):
			dumpvars_set = True
		if search("finish", line):
			finish_set = True
		if search("dumpfile", line):
			dumpfile_set = True

	f.close()

	check_initial(filepath)

	if not dumpfile_set:
		add_dumpfile(filepath)

	if not dumpvars_set:
		add_dumpvars(filepath)

	if not finish_set:
		add_finish(filepath)

def main():
	test_bench_found = False

	chdir(project_settings.getSourceDir())
	for f in glob('*.v'):
		file = open(f, "r")
		for line in file:
			if search(project_settings.getTestBench(), line):
				test_bench_found = True
				file.close()
				check_and_fix(f)
				break;

	if not path.isfile(settings.getIcarus()):
		print("Error: No verilog compiler defined!\n")
		return

	if not test_bench_found:
		print("Error: No test bench defined!\n")
		return

	chdir(project_settings.getSynthesisDir())
	icarus=Popen(
		[
			settings.getIcarus(),
			"-s",project_settings.getTestBench(),
			"-o",project_settings.getTestBench(),
			"-I",project_settings.getSourceDir(),
			project_settings.getTestBenchFile()
		], stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)

	for line in iter(icarus.stdout.readline, ''):
		print line # do something with the output here
	icarus.stdout.close()

	chdir(project_settings.getSynthesisDir())
	binary = path.join(project_settings.getSynthesisDir(), project_settings.getTestBench())
	if path.isfile(binary):
		binaryExec=Popen(binary, stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)
		for line in iter(binaryExec.stdout.readline, ''):
			print line # do something with the output here
		binaryExec.stdout.close()
	else:
		print("Error: No simulator generated!\n")
		return
