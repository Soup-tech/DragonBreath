#!/usr/bin/python3

import os
import subprocess
import sys
import time

def main():

	if (len(sys.argv) < 4):
		usage()
		exit(1)

	print("Creating directory...")
	if (('-o' in sys.argv) or ('--output' in sys.argv)):
		directory = createDirectory()
	else:
		directory = sys.argv[1].split('/')
		del directory[-1]
		directory = ("/".join(directory)) + "/"
		subprocess.Popen("mkdir " + directory+"dragonBreath", shell=True, stderr=subprocess.PIPE)

		directory = directory + "dragonBreath/"

	findText(sys.argv[1], directory)

def findText(binary, directory):

	bianry_path = binary.split("/")
	binary_name = bianry_path[-1]

	print("Finding .text symbols...")
	subprocess.Popen("objdump -t " + binary + " | grep -w .text > " + directory+binary_name+".text",shell=True)

	time.sleep(2)

	if (('-c' in sys.argv) or ('--continue' in sys.argv)):
		print("Restoring save point...")
		restore(directory+binary_name+".text")

	parseGhidra(directory+binary_name+".text")

def restore(text_file):

	if ('-c' in sys.argv):
		c_index = sys.argv.index('-c') + 1
	elif ('--continue' in sys.argv):
		c_index = sys.argv.index('--continue') + 1

	subprocess.Popen("./restore.sh " + text_file + " " + sys.argv[c_index],shell=True)

def parseGhidra(file_location):
	
	omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

	temp_list = file_location.split("/")
	file_name = temp_list[-1]
	del temp_list[-1]

	print("Creating dragonBreathOutput directory...")
	method_directory = "/".join(temp_list) + "/dragonBreathOutput/"
	subprocess.Popen("mkdir " + method_directory, shell=True, stderr=subprocess.PIPE)

	time.sleep(1)

	for line in open(file_location,'r'):
		line = line.strip().split()

		if (line[-1] in omit):
			continue

		# formatting for Ghidra
		address = line[0] + "1"
		address	= address.strip("0")
		address = list(address)
		del address[-1]
		address = "".join(address)

		# ./analyzeHeadless <project directory> <project name> -import <binary name> -postScript GhidraDecompiler.java <function address> -deleteProject
		print("Running Analysis on " + line[-1] + "...")
		subprocess.run(sys.argv[3] + " " + sys.argv[2] + " " + file_name + " -import " + sys.argv[1] + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " + method_directory+line[-1] + " 2> /dev/null", shell=True)


def createDirectory():
	if ('-o' in sys.argv):
		o_switch = sys.argv.index('-o')
	elif ('--output' in sys.argv):
		o_switch = sys.argv.index('--output')

	output_directory = o_switch + 1
	subprocess.Popen("mkdir " + sys.argv[output_directory], shell=True, stderr=subprocess.PIPE)

	directory = sys.argv[output_directory] + "/"
	return directory

def usage():
	print("USAGE: ./dBv2 [path/to/binary] [path/to/ghidra/project] [path/to/analyzeHeadless] [OPTIONS]...[ -c | -o | -h]\n" +
		  "OPTIONS:\n" +
		  "\t-h --help\t: Display this usage message\n" +
		  "\t-o --output\t: Write directory somewhere else\n"
		  "\t-c --continue\t: Continue where dBv2 left off\n")




if __name__ == "__main__":
	main()