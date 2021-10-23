#!/usr/bin/python3

import subprocess
import sys
import time

def main():
	omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

	if (len(sys.argv) < 2):
		usage()
		exit(1)

	# Find location of all executables
	proc = subprocess.Popen("find " + sys.argv[1] + " -type f -executable 2> /dev/null",shell=True,stdout=subprocess.PIPE)
	output = proc.stdout.read().decode('utf-8')
	ex = output.split('\n')
	
	print("Begin Driver...")
	for e in ex: # Drive loop
		if (not e):
			continue

		# Getting folder directory
		e_lst = e.split('/')
		_id = e_lst[-1] # Name of the folder being worked on
		del e_lst[-1]
		folder = "/".join(e_lst) # Top level for the current working folder
		
		print(" + Working on " + _id+"...")

		# Finding symbols
		print("\t+ Finding Symbols...")
		subprocess.Popen("objdump -t " + e + " | grep .text > " + folder+"/Ghidra/"+_id+".text 2> /dev/null",shell=True)

		# Make dragonBreath Directory
		print("\t+ Making Directory's")
		subprocess.Popen("mkdir " + folder+"/Ghidra/dragonBreathOutput 2> /dev/null",shell=True)

		# dragonBreath Driver
		print("\t+ Starting dragonBreath on " + _id)
		for method in open(folder+"/Ghidra/"+_id+".text",'r'):
			method = method.strip().split()

			# Skipping over methods in omit
			if (method[-1] in omit):
				continue

			# Ghidra Format
			address = method[0] + "1"
			address	= address.strip("0")
			address = list(address)
			del address[-1]
			address = "".join(address) # Real address for Ghidra

			# Decompiling
			time.sleep(2)
			print("\t\t+ Decompiling " + method[-1])
			subprocess.run("/opt/Ghidra/support/analyzeHeadless ~/HereBeDragons " + _id + " -import " + e + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " + folder+"/Ghidra/dragonBreathOutput/"+method[-1] + " 2> /dev/null",shell=True)
			


def usage():
	print("USAGE: ./itc_ghidra.py [path/to/itc]")




if __name__ == "__main__":
	main()