#!/usr/bin/python3
#
# This script is built off of guedou's GhidraDecompiler.java script. 
# 
# python3 auto-decompile.py [Top Level Directory] [Ghidra Project Directory] [analyzeHeadless absolute path]
# 

import os
import subprocess
import sys

def main():
    
    if (len(sys.argv) != 4):
        usage()
        exit(1)
    
    getBinary(sys.argv[1])

    # analyzeHeadless absolute path
    analyzeHeadless = sys.argv[3] 

    # The main operation of the script. ex represents the executable, makes the directory for the analysis,
    # makes a list of all the methods that were found, and finally analyzes them
    for ex in open('binaries_list.txt','r'):
        ex = ex.strip()
        makeDirectory(ex)
        getMethods(ex)
        dragonBreath(analyzeHeadless, sys.argv[1], sys.argv[2], ex)

    # os.system("rm binaries_list.txt methods.txt")

def usage():
    print("Usage: python3 auto-decompile.py [Top Level Directory] [Ghidra Project Directory] [analyzeHeadless Absolute Path]")


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
    
    proc = subprocess.Popen("mkdir " + abs_Directory + "src_Ghidra_" + binary + " 2> /dev/null", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    

# This method gets all of the methods for the binary being analyzed and stores them into methods.txt for later use
# @param abs_binary_path The absolute path to the binary being analyzed
def getMethods(abs_binary_path):
    subprocess.run("rm methods.txt 2> /dev/null", shell=True)

    subprocess.run("objdump -t " + abs_binary_path + " | grep .text >> methods.txt 2> /dev/null", shell=True)

    # *** Add and remove what methods should / shouldn't be analyzed here ***
    omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

    for line in open('methods.txt','r'):
        line = line.strip().split()

        if (line[-1] in omit):
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


# Gets the absolute path of all executables in the top level directory
# @return Returns the absolute path to the top level directory as a string
def getBinary(top_level_directory):
    
    proc = subprocess.Popen('cd ' + top_level_directory, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stderr, stdout = proc.communicate()

    if (proc.returncode != 0):
        print("Top level directory does not exist")
        exit(1)
    
    os.system("find " + top_level_directory + " -maxdepth 2 -executable -type f | sort > binaries_list.txt")
    

if __name__ == "__main__":
    main()
