#!/usr/bin/python3
#

import os
import subprocess
import sys

def main():
    
    if (len(sys.argv) < 4):
        usage()
        exit(1)

    if (('-h' in sys.argv) or ('--help' in sys.argv)):
        usage()
        exit(1)
    
    getBinary(sys.argv[1])

    # The main operation of the script. ex represents the executable, makes the directory for the analysis,
    # makes a list of all the methods that were found, and finally analyzes them
    for ex in open('binaries_list.txt','r'):
        ex = ex.strip()
        makeDirectory(ex)
        getMethods(ex)
        dragonBreath(sys.argv[1], sys.argv[2], sys.argv[3], ex)


def usage():
    print("USAGE: python3 auto-decompile.py [Top Level Directory] [Ghidra Project Directory] [analyzeHeadless Absolute Path]\n" +
          "OPTIONS:\n " +
          "\t-h --help\t: Display this Usage Message\n" +
          "\t-q --quiet\t: Do Not Dispaly Ghidra Output\n" +
          "\t-o --output\t: Output logs to a File\n" +
          "\t-c --continue\t: Have dragonBreath pick up where it left off\n" +
          "Note: You can run this script on single executables by replacing Top Level Directory with the absolute path to the\n" +
          "executable.")


# Runs the Ghidra Decompiler analysis on the binary with all the associated parts
# @param analyzeHeadless Absolute path to the analyzeHeadless executable
# @param top_directory The top level directory which contains all the executables
# @param project_directory The absolute path to Ghidra's project directory
# @param abs_binary_path The absolute path to the binary that is being analyzed
def dragonBreath(top_directory, ghidra_project_directory, analyzeHeadless, abs_binary_path):
    # ./analyzeHeadless <project directory> <project name> -import <binary name> -postScript GhidraDecompiler.java <function address> -deleteProject
    
    project = abs_binary_path
    project = abs_binary_path.split("/")
    project_name = project[-1]
    del project[-1]
    project_directory = ("/".join(project)) + "/"

    if (('-q' in sys.argv) or ('--quiet' in sys.argv)):
	    print("===== Started Running Ghidra Analysis On: " + project_name)
	    for line in open(project_directory + 'update_method_list.txt','r'):
	        line = line.strip().split()
	        address = line[0]

	        print("========== Started Running Ghidra Analysis On: " + line[1])
	        
	        try:
	        	subprocess.run(analyzeHeadless + " " + ghidra_project_directory + " " + project_name + " -import " + abs_binary_path + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " + project_directory+"/src_Ghidra_"+project_name+"/"+line[1] + ' 2> /dev/null', shell=True)
	        except:
	        	pass

	        print("========== Finished Decompiling: " + line[1])
    else:
	    print("===== Started Running Ghidra Analysis On: " + project_name)
	    for line in open(project_directory + 'update_method_list.txt','r'):
	        line = line.strip().split()
	        address = line[0]

	        print("========== Started Running Ghidra Analysis On: " + line[1])
	        
	        try:
	      		subprocess.run(analyzeHeadless + " " + ghidra_project_directory + " " + project_name + " -import " + abs_binary_path + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " + project_directory+"/src_Ghidra_"+project_name+"/"+line[1], shell=True)
	        except:
	        	pass
	        	
	        print("========== Finished Decompiling: " + line[1])
	    print("===== Finished Running Ghidra Analysis On: " + project_name)

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
    subprocess.run("rm update_method_list.txt 2> /dev/null", shell=True)
    subprocess.run("objdump -t " + abs_binary_path + " | grep .text >> methods.txt 2> /dev/null", shell=True)
    
    # *** Add and remove what methods shouldn't be analyzed here ***
    omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

    project_path = abs_binary_path.split("/")
    del project_path[-1]
    project_path = ("/".join(project_path)) + "/"

    update_method_file = open(project_path + "update_method_list.txt","w")

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
        update_method_file.write(address_method + '\n')

    os.system("rm methods.txt")
    update_method_file.close()

# Gets the absolute path of all executables in the top level directory
# @return Returns the absolute path to the top level directory as a string
def getBinary(top_level_directory):
    os.system("find " + top_level_directory + " -maxdepth 2 -executable -type f | sort > binaries_list.txt")
    

if __name__ == "__main__":
    main()
