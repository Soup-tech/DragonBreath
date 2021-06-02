#!/usr/bin/python3

import sys
import subprocess

def main():

	if (len(sys.argv) < 3):
		usage()
		exit(1)

	directory_list = pull_directory()

	for folder in directory_list:

		# make appropriate directories
		mkdir_command = "mkdir " + sys.argv[1]+folder+'/src ' + sys.argv[1]+folder+'/Ghidra'	
		subprocess.Popen(mkdir_command, shell=True, stderr=subprocess.PIPE)

	

	


def pull_directory():
	proc = subprocess.Popen("ls " + sys.argv[1], shell=True, stdout=subprocess.PIPE)
	
	directory = proc.stdout.read()

	directory = directory.decode('utf-8')

	dir_lst = directory.split('\n')
	return dir_lst


def usage():
	print("USAGE: ./vuldeepecker_analysis.py [path/to/vuldeepecker/]\n" +
		  "OPTION:\n" +
		  "-d : Create Directories\n" +
		  "-c : Compile Files")





if __name__ == "__main__":
	main()



# proc = subprocess.Popen('ls', stdout=subprocess.PIPE)
# output = proc.stdout.read()
# print output