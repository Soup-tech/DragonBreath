#!/usr/bin/env python3

import sys
import os
import subprocess

argv = sys.argv

def error(msg):
	"""
	Display error messages
	"""
	print(f"Error: {msg}")
	sys.exit(-1)

def isDirecotry(tld):
	"""
	Verifies that tld is a direcotry.
	"""
	if (os.path.isdir(tld)):
		return
	else:
		error(f"{tld} is not a direcotry")

def parseArgs():
	"""
	Parses the arguments that the user supplied in the command line
	"""
	if (len(argv) < 3):
		error("Not enough parameters\n"
			  "Usage: ./flawfinder-ext.py [path/to/source/top_level_directory] [path/to/decompiler/top_level_directory]")

	# Check if the user supplied directories. ## TODO: Enable single file scanning
	source_directory = argv[1]
	isDirecotry(source_directory)

	decompiler_directory = argv[2]
	isDirecotry(source_directory)

	return

def flawfinder():
	"""
	Run the flawfinder 
	"""
	source_csv = open('source.csv','w')
	decomp_csv = open('decomp.csv','w') # TODO: Change the csv name to whatever you are writing
	source_directory = argv[1]
	decompiler_directory = argv[2]

	# Run flawfinder for both decompiled and source code
	# Right now I just reference the flawfinder in my repo bc that's the one I keep modifying
	print(f"Scanning {source_directory}")
	subprocess.Popen(['/home/maximillian/Documents/repos/DragonBreath/src/ASAT/FlawFinder/flawfinder','--csv',source_directory],stdout=source_csv)

	print(f"Scanning {decompiler_directory}")
	subprocess.Popen(['/home/maximillian/Documents/repos/DragonBreath/src/ASAT/FlawFinder/flawfinder','--csv',decompiler_directory],stdout=decomp_csv)

	source_csv.close()
	decomp_csv.close()

def main():
	# Flawfinder results
	parseArgs()
	flawfinder()
	


if __name__ == "__main__":
	main()