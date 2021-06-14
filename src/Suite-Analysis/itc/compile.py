#!/usr/bin/python3

# ./compile.py [path/to/suite] [path/to/manifest]

import sys
import subprocess
import time

def main():
	if (len(sys.argv) < 3):
		usage()
		exit(1)

	# Getting file id and gcc command to compile
	print("compile.py Has Started...")
	for line in open(sys.argv[2],'r'):
		line = line.strip()
		if ('instruction' in line):
			line_lst = line.split('"')
			
			# Command to run in the file and the file id 
			gcc = line_lst[11]
			file = line_lst[1]

			# Cleaning up file id
			file = list(file)
			del file[:3]
			file = "".join(file)

			time.sleep(2)
			print(" + Compiling " + file)
			print(" + gcc Command " + gcc)
			subprocess.Popen("./compile.sh " + sys.argv[1] + " " + file + " " + gcc,shell=True)

def usage():
	print("USAGE: ./compile.py [path/to/suite] [path/to/manifest]")



if __name__ == "__main__":
	main()