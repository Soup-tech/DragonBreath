#!/usr/bin/python3

import subprocess
import sys
import os

def main():
	# path to retdec
	# path to top level directory 
	# ./handsOnDeck.py /path/to/retdec-decompile.py /path/to/top/level/directory

	getBinary(sys.argv[2])

	for ex in open('retdec_binary_List.txt','r'):
		ex = ex.strip()
		makeDirectory(ex)
		run_retdec(ex, sys.argv[1])

def run_retdec(abs_binary_path, retdec_abs_path):
	output_dir = abs_binary_path.split("/")
	binary_name = output_dir[-1]
	del output_dir[-1]
	output_dir = ("/".join(output_dir)) + "/"

	subprocess.run(retdec_abs_path + " " + abs_binary_path + " -o " + output_dir+"src_retdec_"+binary_name+"/"+binary_name + " > " + output_dir+"src_retdec_"+binary_name+"/retdec_output.log" + " 2>" + output_dir+"src_retdec_"+binary_name+"/retdec_output.log", shell=True)


def makeDirectory(executable_abs_path):
	ex_directory = executable_abs_path.split("/")
	binary_name = ex_directory[-1]
	del ex_directory[-1]
	ex_directory = ("/".join(ex_directory)) + "/"

	proc = subprocess.Popen("mkdir " + ex_directory + "src_retdec_" + binary_name + " 2> /dev/null", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def getBinary(top_directory):
	os.system("find " + top_directory + " -maxdepth 2 -executable -type f | sort > retdec_binary_List.txt")




if __name__ == "__main__":
	main()