#!/usr/bin/python3
#
# ./analyzeHeadless <Project Directory> <Project Name> -import <Binary Name> -postScript GhidraDecompiler.java <Function Address> -deleteProject

import os
import subprocess
import itertools

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

    analyzeHeadless = input("Specify analyzeHeadless absolute path: ")

    for ex in binary_list:
        ex = ex.strip()
        makeDirectory(ex)
        getMethods(ex)
        dragonBreath(analyzeHeadless, top_directory, project_directory, ex)

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

def makeDirectory(abs_binary_path):
    myList = abs_binary_path.split('/')
    binary = myList[-1]
    del myList[-1]
    abs_Directory = "/".join(myList)
    abs_Directory = abs_Directory + "/"
    
    subprocess.run("mkdir " + abs_Directory + "src_Ghidra_" + binary + " 2> /dev/null", shell=True)

def getMethods(abs_binary_path):
    subprocess.run("objdump -t " + abs_binary_path + " | grep .text >> methods.txt 2> /dev/null", shell=True)
    
    method_file = open("methods.txt","r")
    method_lst = method_file.readlines()
    method_file.close()
    os.system("rm methods.txt")

    method_file = open("methods.txt","w")
    address_method = []
    for line in method_lst:
        line = line.strip().split()
        
        # Formatting Ghidra address
        address = line[0] + "1"
        address = address.strip("0")
        address = list(address)
        del address[-1]
        address = "".join(address)
        
        # Writing to methods.txt
        address_method = address + " " + line[-1]
        method_file.write(address_method + '\n')


    method_file.close()

# Gets the absolute path of all executables in the top level directory
def getBinary():
    try:
        top_directory = input("Specify top level directory: ")
        exist = subprocess.run("cd " + top_directory, stderr=subprocess.STDOUT, shell=True)
    except:
        exist = "DNE"

    if (exist == "DNE"):
        print("Could not find directory...")
        exit(1)

    os.system("find " + top_directory + " -maxdepth 2 -executable -type f | sort > binaries_list.txt")
    return top_directory    


if __name__ == "__main__":
    main()
