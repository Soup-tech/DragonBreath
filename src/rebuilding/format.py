#!/usr/bin/python3

import sys
import os
import subprocess

def main():
	ghidra_raw = ""

	if (len(sys.argv) != 2):
		usage()
		exit(1)

	data_types = ["void","uchar"]

	top_dir = sys.argv[1].split('/')
	del top_dir[-2:]
	top_dir = "/".join(top_dir) + '/'
	
	subprocess.Popen("mkdir " + top_dir+"GhidraSource", shell=True, stderr=subprocess.PIPE)
	
	print("Creating dragonBreath.lst...")
	subprocess.Popen("ls " + sys.argv[1] + " > dragonBreath.lst", shell=True, stderr=subprocess.PIPE)

	for line in open("dragonBreath.lst","r"):
		line = line.strip()
		print("Formatting " + line+"...")
		subprocess.Popen('./ghidra-format.sh ' + sys.argv[1]+line + " " + top_dir+'GhidraSource/'+line+".c",shell=True,stderr=subprocess.PIPE,stdout=subprocess.PIPE)
	
	
	#subprocess.Popen("rm dragonBreath.lst",shell=True)


def usage():
	print("USAGE: ./format.py [path/to/ghidra_source/directory]")

if __name__ == "__main__":
	main()