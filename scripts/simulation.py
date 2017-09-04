from os import path
from os import popen
from os import listdir
from os import remove
from re import search
from shutil import move

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
					n.write("\ninitial begin\n\nend\n")
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
	dumpvars_name = project_settings.getTestBench() + ".vcd"
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

def simulation():
	source = project_settings.getSourceDir() 
	source += "/*.v"

	binary = project_settings.getSynthesisDir() 
	binary += "/"
	binary += project_settings.getTestBench()

        if path(source).isfile():

            for f in listdir(project_settings.getSourceDir()):
                    filepath=project_settings.getSourceDir()+'/'+f
                    file = open(filepath, "r")
                    for line in file:
                            if search(project_settings.getTestBench(), line):
                                    print line
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
            print(popen(binary).read())

            move(project_settings.getVCDFile(),project_settings.getVCDPath())

        else:
            print("Error: File "+source+" doesn't exist\n")

simulation()
