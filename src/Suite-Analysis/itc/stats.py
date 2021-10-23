#!/usr/bin/env python3

import sys

argv = sys.argv
final_stats = {"src":{}, "src_Ghidra":{}, "src_RetDec":{}, "src_JEB":{}, "src_IDA":{}}

def usage():
	print("USAGE: ./stats.py [path/to/file.csv] [OPTIONS]...[ -l | -c | -n ]")
	print("\t-l : Get stats for the level")
	print("\t-c : Get stats for the category")
	print("\t-n : Get stats for the name\n")

# Correct num of params?
if (len(argv) != 3):
	usage()
	sys.exit(-1)

# Determine what is being counted
switch = argv[2]
if (switch == '-l'):
	seg = 1
elif (switch == '-c'):
	seg = 2
elif (switch == '-n'):
	seg = 3
else:
	print("Error: Unknown switch")
	sys.exit(-1)

# Iterate through csv and gather specified information
for line in open(argv[1],'r'):
	line = line.strip().split(',')
	# Skip over header
	if ("File" in line):
		continue
	
	# Grab info that is being counted
	info = line[seg]
	source = line[0]

	if (source in final_stats.keys()): # if the source is Ghidra, RetDec, JEB, or IDA
		
		if (info not in final_stats[source].keys()):
			final_stats[source][info] = 1
		else:
			final_stats[source][info] += 1

	else: # Original Source

		if (info not in final_stats['src'].keys()):
			final_stats['src'][info] = 1
		else:
			final_stats['src'][info] += 1

# Display the information
for src,stats in final_stats.items():
	print("{}".format(src))
	for counted,counters in stats.items():
		print("\t{} : {}".format(counted,counters))