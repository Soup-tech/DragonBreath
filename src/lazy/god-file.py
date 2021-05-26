#!/usr/bin/python3

import sys
import os
import subprocess

def main():
	
	if (len(sys.argv) < 2):
		usage()
		exit(1)

	os.system("ls " + sys.argv[1] + "> methods_list.txt")

	# parsing through ghidra results
	for line in open("methods_list.txt",'r'):
		line = line.strip()
		print("Adding " + line + "...")

		if (line == 'main'):
			is_main = subprocess.Popen("cat " + sys.argv[1]+line, shell=True, stdout=subprocess.PIPE)
			pass

		f = subprocess.Popen("cat " + sys.argv[1]+line, shell=True, stdout=subprocess.PIPE)
		getSource(f,line)

	print("Adding main...")
	getSource(is_main, 'main')


def getSource(dragonbreath_output, method_name):
	god_file = open("god_file.c",'a')
	failure_file = open("not_added.txt",'a')

	b = dragonbreath_output.stdout.read()
	output = b.decode('UTF-8')

	try:
		no_disass = output.split("INFO  GhidraDecompiler.java> Decompilation completed: true (GhidraScript)")
		no_disass = no_disass[1].split(" (GhidraScript)  ")
		no_disass = no_disass[0].split("INFO  GhidraDecompiler.java> ")
		god_file.write(no_disass[1])
	except:
		failure_file.write(method_name + '\n')

	failure_file.close()
	god_file.close()


def usage():
	print("USAGE: ./god-file.py [path/to/Ghidra/method/directory/")




if __name__ == "__main__":
	main()