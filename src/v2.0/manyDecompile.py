#!/usr/bin/python3

import sys
import subprocess

class manyDecompile:
    # Constructor
    def __init__(self):
        self.top_level_directory = ""
        self.binary_path = "file"
        self.decompiler_path = ""
        self.ghidra_project_folder = ""
        self.omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]
        self.methods = []
        self.binaries = []

    # Setters
    def setDirectory(self, directory):
        self.top_level_directory = directory
        return True
    
    def setBinaryPath(self, bin_path):
        self.binary_path = bin_path
        return True
        
    def setDecompiler(self, decomp_path):
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

    # Used for debugging
    def printAll(self):
        print("Top Level Directory : " + self.top_level_directory)
        print("Current Binary : " + self.binary_path)
        print("Decompiler : " + self.decompiler_path)
        print("Ghirda Project Folder : " + self.ghidra_project_folder)
        print("Methods : " + str(self.methods))

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

    def appendBinary(self, binary):
        if (binary in self.binaries):
            print(binary + " already exists")
            return False
        else:
            self.binaries.append(binary)

    def getBinaries(self):
        if (not self.top_level_directory):
            print("Error: Top level directory not set")
            return False

        print("Would you like to save the binaries to a file? [y/n]")
        decision = input("> ")

        if (decision == 'y'):
            subprocess.run("find " + self.top_level_directory + " -maxdepth 2 -executable -type f | sort > binary_list.txt" ,shell=True)
            self.binaries = "file"
        elif (decision == 'n'):
            self.binaries = subprocess.run("find -maxdepth 2 -executable -type f | sort",shell=True,captrue_output=True)
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

        if (decision == 'y'):
            update_method_list.close()

        return True

    def makeDirectories(self):
        if (not self.binary_path):
            print("Error: Absolute binary path not set")
            return False
        elif (not self.decompiler_path):
            print("Error: Must select a decompiler first")
            return False

        # Sanitizing directory path
        home_dir = self.binary_path.split("/")
        binary_name = home_dir[-1]
        del home_dir[-1]
        home_dir = ("/".join(home_dir)) + "/"

        # Getting Decompiler name
        decompiler = self.decompiler_path.split("/")[-1]


        if (decompiler == "retdec-decompiler.py"):
            subprocess.run("mkdir " + home_dir+"src_retdec_"+binary_name + " 2> /dev/null",shell=True)
        elif (decompiler == "analyzeHeadless"):
            subprocess.run("mkdir " + home_dir+"src_Ghidra_"+binary_name + " 2> /dev/null",shell=True)

        return True

    def decompile(self):
        if (not self.decompiler):
            print("Error: Decompiler not set")
            return False
        
        decompiler = (self.decompiler_path.split("/"))[-1]

        if ((decompiler == "analyzeHeadless") and (not self.ghidra_project_folder)):
            print("Error: If you are using Ghirda, set the project folder path")
            return False

        if (decompiler == "analyzeHeadless"):
            dragonBreath()
        elif (decompiler == "retdec-decompiler.py"):
            matey()


    #def dragonBreath(self):


    #def matey(self):


