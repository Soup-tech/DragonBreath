#!/usr/bin/env python

import sys
import os


def usage():
	print("\nUSAGE: ./format.py [path/to/csv]")
	print("Filters unwanted information out of raw Flawfinder csv files\n")

def parseContext(line):
	comma_split = line.split(",")

	for i in range(len(comma_split) - 1, 0, -1):
		 # Iterating backwards through the list until it hits something with CWE
		 # Then rebuilds up to the signature
		if ('CWE' in comma_split[i]):
			context = comma_split[i+1:-1]
			context = ",".join(context)
			context = context.strip('"').strip()
			return '"' + context + '"'


argv = sys.argv

if (len(argv) != 2):
	usage()
	sys.exit(-1)

# Create file header
outputFile = open('results.csv','w')
outputFile.write("File,Level,Category,Name,Context\n")

for line in open(argv[1],'r'): # Iterate through raw csv file

	# Format the context (commas in the context make it difficult)
	context = parseContext(line)

	line = line.strip().split(',')
	if ('File' in line): # Skip header
		continue

	# Get wanted information here
	file = line[0]
	level = line[3]
	category = line[4]
	name = line[5]

	# Format the wanted info and write it to a file
	finLine = "{},{},{},{},{}\n".format(file,level,category,name,context)
	outputFile.write(finLine)

outputFile.close()