import io
import time
from subprocess import PIPE
from subprocess import STDOUT
from subprocess import Popen
import sys

def main():
	p=Popen(settings.getYosys(), stdout=PIPE, stdin=PIPE, stderr=STDOUT, bufsize=1)
	p.stdin.write("exit\n")
	p.stdin.close() # eof

	for line in iter(p.stdout.readline, ''):
		print line, # do something with the output here
	p.stdout.close()
