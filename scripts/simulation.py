from os import popen
from os import listdir
from os import remove
from re import search
from shutil import move


def add_before_end(filepath, lines):
	f = open(filepath,'r')
	n = open(filepath+".temp",'w')

	for i in f:
		if i.find('initial begin') == -1:
			n.write(i)
			continue

		n.write(i) 
		n.write(lines) 

	f.close()
	n.close()

	remove(filepath)
	move(filepath+".temp",filepath)

def add_initial(filepath):
	f = open(filepath,'r')
	n = open(filepath+".temp",'w')

	for i in f:
		if i.find('endmodule') == -1:
			n.write(i)
			continue

		n.write("initial begin\n\nend\n\n") 
		n.write(i) 

	f.close()
	n.close()

	remove(filepath)
	move(filepath+".temp",filepath)

def check_initial(filepath):
	f = open(filepath, "r")
	initial_set = False

	for line in f:
		if search("initial", line):
			initial_set = True

	if not initial_set:
		add_initial(filepath)

def add_dumpvars(filepath):
	print "dumpvars missing"
	check_initial(filepath)
	add_before_end(filepath, "\n# 1500 $dumpvars;\n");

def add_finish(filepath):
	print "finish missing"
	check_initial(filepath)
	add_before_end(filepath, "\n# 1501 $finish;\n");

def check_and_fix(filepath):
	f = open(filepath, "r")
	dumpvars_set = False
	finish_set = False

	for line in f:
		if search("dumpvars", line):
			dumpvars_set = True
		if search("finish", line):
			finish_set = True

	if not finish_set:
		add_finish(filepath)

	if not dumpvars_set:
		add_dumpvars(filepath)

def simulation():
	source = project_settings.getSourceDir() 
	source += "/*.v"

	binary = project_settings.getSynthesisDir() 
	binary += "/"
	binary += project_settings.getTestBench()

	for f in listdir(project_settings.getSourceDir()):
		filepath=project_settings.getSourceDir()+'/'+f
		file = open(filepath, "r")
		for line in file:
			if search(project_settings.getTestBench(), line):
				check_and_fix(filepath)
				break;

	simulationCommand = settings.getIcarus()
	simulationCommand += " -s "
	simulationCommand += project_settings.getTestBench()
	simulationCommand += " "
	simulationCommand += source
	simulationCommand += " -o "
	simulationCommand += binary

	print(popen(simulationCommand).read())

simulation()
