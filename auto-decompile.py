#!/usr/bin/python3
#
# This script is built off of guedou's GhidraDecompiler.java script. 
# 
# Some tweaking to meet personal needs is the depth of the top level directory and methods to omit. This will be clearly marked within this source code. 
#

import os
import subprocess
import sys

def main():
    
    # Get all binary locations from top-level directory
    top_directory = getBinary()
    
    # Getting Ghidra's project directory location
    try:
        project_directory = input("Specify Ghidra project directory: ")
        exist = subprocess.run("cd " + project_directory, stderr=subprocess.STDOUT, shell=True)
    except:
        exist = "DNE"

    if (exist == "DNE"): # Could not find Ghidra's project directory
        print("Could not find project directory...")
        exit(1)
    
    # Create directories for each binary to store analyzed methods   
    binary_file = open("binaries_list.txt","r")
    binary_list = binary_file.readlines()
    binary_file.close()

    # analyzeHeadless absolute path
    analyzeHeadless = input("Specify analyzeHeadless absolute path: ")

    # The main operation of the script. ex represents the executable, makes the directory for the analysis,
    # makes a list of all the methods that were found, and finally analyzes them
    for ex in binary_list:
        ex = ex.strip()
        makeDirectory(ex)
        getMethods(ex)
        dragonBreath(analyzeHeadless, top_directory, project_directory, ex)

    os.system("rm binaries_list.txt methods.txt")


# Runs the Ghidra Decompiler analysis on the binary with all the associated parts
# @param analyzeHeadless Absolute path to the analyzeHeadless executable
# @param top_directory The top level directory which contains all the executables
# @param project_directory The absolute path to Ghidra's project directory
# @param abs_binary_path The absolute path to the binary that is being analyzed
def dragonBreath(analyzeHeadless, top_directory, project_directory, abs_binary_path):
    # ./analyzeHeadless <project directory> <project name> -import <binary name> -postScript GhidraDecompiler.java <function address> -deleteProject
    
    method_file = open("methods.txt","r")
    method_list = method_file.readlines()
    method_file.close()

    # Project name / Binary 
    binary_path = abs_binary_path.split("/")
    
    project_name = binary_path[-1]
    del binary_path[-1]
    binary_path = "/".join(binary_path)

    print("----- Started running Ghidra analysis on: " + project_name)
    for line in method_list:
        line = line.strip().split()
        address = line[0]
        print("---------- Started Decompiling: " + line[1])
        subprocess.run(analyzeHeadless + " " + project_directory + " " + project_name + " -import " + abs_binary_path + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " 
                                  + binary_path + "/src_Ghidra_" + project_name + "/" + line[1], shell=True)
        print("---------- Finished Decompling: " + line[1])
    print("----- Finished running Ghidra analysis on: " + project_name)

    os.system("rm methods.txt")

# This method makes a directory for every executable and will store Ghidra's analysis
# of the method within this directory
# @param abs_binary_path The absolute path to the binary being analyzed
def makeDirectory(abs_binary_path):
    
    myList = abs_binary_path.split('/')
    binary = myList[-1]
    del myList[-1]
    abs_Directory = "/".join(myList)
    abs_Directory = abs_Directory + "/"
    
    subprocess.run("mkdir " + abs_Directory + "src_Ghidra_" + binary + " 2> /dev/null", shell=True)

# This method gets all of the methods for the binary being analyzed and stores them into methods.txt for later use
# @param abs_binary_path The absolute path to the binary being analyzed
def getMethods(abs_binary_path):
    subprocess.run("rm methods.txt 2> /dev/null", shell=True)

    subprocess.run("objdump -t " + abs_binary_path + " | grep .text >> methods.txt 2> /dev/null", shell=True)

    # *** Add and remove what methods should / shouldn't be analyzed here ***
    omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

    method_file = open("methods.txt","r")
    method_lst = method_file.readlines()
    method_file.close()
    os.system("rm methods.txt")

    method_file = open("methods.txt","w")
    address_method = []
    i = 0
    for line in method_lst:
        line = line.strip().split()
        
        if line[-1] in omit:
            continue

        # Formatting Ghidra address
        address = line[0] + "1"
        address = address.strip("0")
        address = list(address)
        del address[-1]
        address = "".join(address)
        
        # Writing to methods.txt
        address_method = address + " " + line[-1]
        method_file.write(address_method + '\n')
        
        i += 1

    method_file.close()

# Gets the absolute path of all executables in the top level directory
# @return Returns the absolute path to the top level directory as a string
def getBinary():
    try:
        top_directory = input("Specify top level directory: ")
        exist = subprocess.run("cd " + top_directory, stderr=subprocess.STDOUT, shell=True)
    except:
        exist = "DNE"

    if (exist == "DNE"):
        print("Could not find top-level directory...")
        exit(1)

    os.system("find " + top_directory + " -maxdepth 2 -executable -type f | sort > binaries_list.txt")
    return top_directory    


if __name__ == "__main__":
    main()
