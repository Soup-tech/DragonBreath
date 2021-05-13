#!/usr/bin/python3

import sys
import subprocess

class manyDecompile:
	# Constructor
	def __init__(self):
		self.top_level_directory = ""
		self.binary_path = ""
		self.decompiler_path = ""
		self.ghidra_project_folder = ""
		self.omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]
		self.methods = []

	# Setters
	def setDirectory(self, directory):
		self.top_level_directory = directory
		return True
	
	def setBinaryPath(self, bin_path):
		self.binary_path = bin_path
		return True
	
	def setDecompiler(set, decomp_path):
		self.decompiler_path = decomp_path
		return True
	
	def setGhidraProject(self, g_proj_folder):
		self.ghidra_project_folder = g_proj_folder
		return True

	# Getters
	def getDirectory(self):
		return self.top_level_directory
	
	def getBinaryPath(self):
		return self.binary_path
	
	def getDecompiler(self):
		return self.decompiler_path
	
	def getGhidraProject(self):
		return self.ghidra_project_folder
	
	def getOmit(self):
		return self.omit

	def getMethods(self):
		return self.methods

	def appendOmit(self, omit_method):
		if (omit_method in self.omit):
			print(omit_method + " already omitted")
			return False
		else:
			self.omit.append(omit_method)
			return True

	def delOmit(self, omit_method):
		if (omit_method not in self.omit):
			print(omit_method + " not found")
			return False
		else:
			self.omit.remove(omit_method)
			return True

	def fetchMethods(self):
		if (not self.binary_path): # Is binary_path set?
			print("Error: Absolute binary path not set")
			return False
		
		print("Would you like to save the methods to a file? [y/n]")
		decision = input("> ")

		if (decision == 'y'):
			update_method_list = open("methods.txt","w")


		print("Fetching Methods...")
		subprocess.run("objdump -t " + self.binary_path + " | grep .text >> methods.raw 2> /dev/null",shell=True)
		

		for line in open("methods.raw","r"):
			line = line.strip().split()

			if (line[-1] in self.omit):
				continue

			# Formatting Ghidra address
	        address = line[0] + "1"
	        address = address.strip("0")
	        address = list(address)
	        del address[-1]
	        address = "".join(address)

	        if (decision == 'y'):
	        	update_method_list.write(address + " " + line[-1] + "\n")
	        else:
	        	self.methods.append(address + " " + line[-1])

	    subprocess.run("rm methods.raw 2> /dev/null",shell=True)
	    update_method_list.close()

		return True



