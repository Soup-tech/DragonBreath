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

def makeDirectory():
	if (('-m' in sys.argv)):
		m_index = sys.argv.index('-m') + 1
	elif (('--make' in sys.argv)):
		m_index = sys.argv.index('--make') + 1
		
	dirs = sys.argv[m_index].split(',')
	for d in dirs:
		subprocess.Popen("./create_folders.sh " + d,shell=True,stderr=subprocess.PIPE)

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
		  "\t-m --make\t: Make directory's in all folders")




if __name__ == "__main__":
	main()



# proc = subprocess.Popen('ls', stdout=subprocess.PIPE)
# output = proc.stdout.read()
# print output