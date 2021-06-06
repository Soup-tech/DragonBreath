#!/usr/bin/python3

import sys
import subprocess

def main():

	if ((len(sys.argv) < 2) or ('-h' in sys.argv) or ('--help' in sys.argv)):
		usage()
		exit(1)

	if (('-c' in sys.argv) or ('--compile' in sys.argv)):
		runCompile(sys.argv[1])
	elif (('-o' in sys.argv) or ('--output' in sys.argv)):
		outputToDir(sys.argv[1])
	elif (('-m' in sys.argv) or ('--make' in sys.argv)):
		makeDirectory()
	elif (('-d' in sys.argv) or ('--dragon' in sys.argv)):
		dragon()
	elif (('-f' in sys.argv) or ('--format' in sys.argv)):
		formatVul()

def formatVul():
	# Find directories contaning output files
	files = []
	print("Finding files....")
	proc = subprocess.Popen("find " + sys.argv[1] + " -type d -name 'dragonBreathOutput'", shell=True, stdout=subprocess.PIPE)
	output = proc.stdout.read().decode('utf-8')
	output = output.split('\n')

	for line in output:
		line = line.strip()
		
		# Get all output files
		ls = subprocess.Popen("ls " + line,shell=True,stdout=subprocess.PIPE)
		try:
			files = ls.stdout.read().decode('utf-8')
			files = files.split('\n')
		except:
			continue

		for f in files:
			if (not f):
				continue

			# Formatting for output directory
			output_dir = line.split('/')
			name = output_dir[-3]
			del output_dir[-1]
			output_dir = "/".join(output_dir) + "/"

			subprocess.Popen("mkdir " + output_dir+'src_Ghidra/' + " 2> /dev/null",shell=True)
			print(" + Cleaning up " + name + "...")
			try:
				subprocess.Popen("./format-vul.sh " + line+'/'+f + " " + output_dir+'src_Ghidra/'+f+'.c' + " 2> /dev/null", shell=True)
			except:
				continue


def dragon():
	omit = ["deregister_tm_clones", "register_tm_clones","__do_global_dtors_aux","frame_dummy","__libc_csu_fini","__libc_csu_init","_start",".text"]

	print("Finding executables...")
	ex = subprocess.Popen("find " + sys.argv[1] + " -type f -executable",shell=True,stdout=subprocess.PIPE)
	ex = ex.stdout.read().decode('utf-8')
	ex = ex.split("\n")
	
	for e in ex:
		# Binary Location
		binary_location = e.strip()

		# Ghidra home directory / binary name
		e = e.strip().split('/')
		dir_name = e[-2]
		bin_name = e[-1]
		del e[-1]
		e = "/".join(e) + "/Ghidra/"
		
		print(" + Starting analysis on " + dir_name + "...")
		text_file = e + "/"+bin_name+".text"
		for method in open(text_file,'r'):
			method = method.strip().split()

			# Skipping over methods in omit
			if (method[-1] in omit):
				continue

			# Ghidra Format
			address = method[0] + "1"
			address	= address.strip("0")
			address = list(address)
			del address[-1]
			address = "".join(address)

			# ./analyzeHeadless <project directory> <project name> -import <binary name> -postScript GhidraDecompiler.java <function address> -deleteProject
			print("\t+ Analyzing " + method[-1])
			subprocess.run("/opt/Ghidra/support/analyzeHeadless ~/HereBeDragons " + bin_name + " -import " + binary_location + " -postScript GhidraDecompiler.java " + "10"+address + " -deleteProject > " + e+"dragonBreathOutput/"+method[-1] + " 2> /dev/null", shell=True)




def makeDirectory():
	if (('-m' in sys.argv)):
		m_index = sys.argv.index('-m') + 1
	elif (('--make' in sys.argv)):
		m_index = sys.argv.index('--make') + 1
		
	dirs = sys.argv[m_index].split(',')
	for d in dirs:
		subprocess.Popen("./create_folders.sh " + sys.argv[1] + " " + d,shell=True,stderr=subprocess.PIPE)

def outputToDir(vuldeepecker_directory):
	# Find index
	if ('-o' in sys.argv):
		o_index = sys.argv.index('-o') + 1
	elif ('--output' in sys.argv):
		o_index = sys.argv.index('--output') + 1

	proc = subprocess.Popen("find " + vuldeepecker_directory + " -type f -executable",shell=True,stdout=subprocess.PIPE)
	output = proc.stdout.read()
	output = output.decode('utf-8')

	output = output.split('\n')

	for ex in output:
		ex = ex.strip().split('/')
		del ex[-1]
		ex = "/".join(ex)
		
		subprocess.Popen("mv " + ex + " " + sys.argv[o_index], shell=True, stderr=subprocess.PIPE)

def runCompile(vuldeepecker_directory):
	subprocess.Popen("./compile.sh " + vuldeepecker_directory, shell=True)


def usage():
	print("USAGE: ./vuldeepecker_analysis.py [path/to/vuldeepecker/] [OPTIONS]...[-c | -o | -h]\n" +
		  "OPTIONS:\n" +
		  "\t-h --help\t: Display this help message\n" +
		  "\t-c --compile\t: Compile all possible files\n" +
		  "\t-o --output\t: Output all successfully compiled files to a directory\n" +
		  "\t-m --make\t: Make directory's in all folders\n" +
		  "\t-d --dragon\t: Run Ghidra on all executables")




if __name__ == "__main__":
	main()



# proc = subprocess.Popen('ls', stdout=subprocess.PIPE)
# output = proc.stdout.read()
# print output