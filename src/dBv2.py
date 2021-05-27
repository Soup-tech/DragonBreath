#!/usr/bin/python3

import os
import subprocess
import sys

def main():

	if (len(sys.argv) < 4):
		usage()
		exit(1)

	if (('-o' in sys.argv) or ('--output' in sys.argv)):
		directory = createDirectory()
	else:
		directory = sys.argv[1].split('/')
		del directory[-1]
		directory = ("/".join(directory)) + "/"
		subprocess.Popen("mkdir " + directory+"dragonBreath", shell=True)

		directory = directory + "dragonBreath/"

	subprocess.Popen("mkdir " + directory+"bss", shell=True,stderr=subprocess.PIPE)
	findBss(sys.argv[1], directory)

	subprocess.Popen("mkdir " + directory+"data", shell=True, stderr=subprocess.PIPE)
	findData(sys.argv[1], directory)

	subprocess.Popen("mkdir " + directory+"text", shell=True, stderr=subprocess.PIPE)
	findText(sys.argv[1], directory)

	dragonBreath()

def findText(binary, directory):

	bianry_path = binary.split("/")
	binary_name = bianry_path[-1]

	subprocess.Popen("objdump -t " + binary + " | grep -w .text > " + directory+"text/"+binary_name+".text",shell=True)
	parseGhidra(directory+"text/"+binary_name+".text")



def findData(binary, directory):
	
	bianry_path = binary.split("/")
	binary_name = bianry_path[-1]

	subprocess.Popen("objdump -t " + binary + " | grep -w .data > " + directory+"data/"+binary_name+".data",shell=True)
	parseGhidra(directory+"data/"+binary_name+".data")


def findBss(binary, directory):

	bianry_path = binary.split("/")
	binary_name = bianry_path[-1]

	subprocess.Popen("objdump -t " + binary + " | grep -w .bss > " + directory+"bss/"+binary_name+".bss", shell=True)

	parseGhidra(directory+"bss/"+binary_name+".bss")


def parseGhidra(file_location):

	temp_list = file_location.split("/")
	file_name = temp_list[-1]
	del temp_list[-1]
	file_name = "/updated-"+file_name

	method_directory = "/".join(temp_list) + "/ghidraOutput/"
	subprocess.Popen("mkdir " + method_directory, shell=True, stderr=subprocess.PIPE)

	for line in open(file_location,'r'):
		line = line.strip().split()

		# formatting for Ghidra
		address = line[0] + "1"
		address	= address.strip("0")
		address = list(address)
		del address[-1]
		address = "".join(address)

		# ./analyzeHeadless <project directory> <project name> -import <binary name> -postScript GhidraDecompiler.java <function address> -deleteProject
		print("Running Analysis on " + line[-1] + "...")
		subprocess.Popen(sys.argv[3] + " " + sys.argv[2] + " " + file_name + " -import " + sys.argv[1] + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject 2> /dev/null > " + method_directory+line[-1], shell=True)

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
	print("USAGE: ./dBv2 [path/to/binary] [path/to/ghidra/project] [path/to/analyzeHeadless]\n" +
		  "OPTIONS:\n" +
		  "\t-h --help\t: Display this usage message\n" +
		  "\t-o --output\t: Write directory somewhere else\n")




if __name__ == "__main__":
	main()