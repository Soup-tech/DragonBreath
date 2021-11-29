#!/usr/bin/env python3

import sys
import os
import subprocess

argv = sys.argv

### Structures
# {method: {name: {source_context: [{probability:decomp_info},{probability:decomp_info},...] } } }
hit_list = {}
# source_line_information = {uid:[hit_line,beginning_line,end_line],uid:[hit_line,beginning_line,end_line] }
source_line_information = {}

### Switches
## Display debugging information for the source data
debug_build = False
## Display debugging information for the decompiled data
debug_batch = False
## Display debugging infromation for the locality data structures
debug_locality = False

### Coefficients
# Weighted average for batching. Default = 1
batch_coefficient = 1
# Weighted average for locality. Default = 
locality_coefficient = 1

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

def getUsableSwitchInfo(switch):
	return argv.index(switch) + 1

def parseArgs():
	"""
	Parses the arguments that the user supplied in the command line
	"""
	global debug_build
	global debug_batch
	global debug_locality

	if (len(argv) < 3):
		error("Not enough parameters\n"
			  "Usage: ./flawfinder-ext.py [path/to/source/top_level_directory] [path/to/decompiler/top_level_directory]")

	# Check if the user supplied directories. ## TODO: Enable single file scanning
	source_directory = argv[1]
	isDirecotry(source_directory)

	decompiler_directory = argv[2]
	isDirecotry(source_directory)

	if ('--debug' in argv):
		# See debug information
		debug_index = getUsableSwitchInfo('--debug')
		try:
			if (argv[debug_index] == 'build'):
				debug_build = True
			elif (argv[debug_index] == 'batch-average'):
				debug_batch = True
			elif (argv[debug_index] == 'locality'):
				debug_locality = True
		except:
			error("Supply debugging option: 'build', 'batch-average', 'locality'")

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
	subprocess.call(['/home/maximillian/Documents/repos/DragonBreath/src/ASAT/FlawFinder/flawfinder','--csv',source_directory],stdout=source_csv)

	print(f"Scanning {decompiler_directory}")
	subprocess.call(['/home/maximillian/Documents/repos/DragonBreath/src/ASAT/FlawFinder/flawfinder','--csv',decompiler_directory],stdout=decomp_csv)

	source_csv.close()
	decomp_csv.close()

def parseMethod(line):
	"""
	Parse the method into the name
	"""
	method_name = line.split('(')[0].split()[1]
	return method_name

def methodFile(c_source,method_file):
	"""
	Produces a file with every method in a file. Each line will have the method name, beginning line number, ending
	line number and total number of lines in the method. 
	"""
	curly_count = 0
	ref_line_beginning = 0
	ref_line_ending = 0

	c_file = open(c_source,'r').readlines()

	for line_num,line in enumerate(c_file):
		line = line.strip()

		# Comes across an opening curly brace and is currently not in a method
		if ('{' in line and curly_count == 0):
			ref_line_beginning = line_num
			curly_count += 1

		# For every opening curly brace, the counter is incremented
		elif ('{' in line):
			curly_count += 1

		# For every closing curly brace, the counter is decremented
		elif ('}' in line):
			curly_count -= 1

		# When reaching the end of a method, the curly count should be at zero.
		if (curly_count == 0 and '}' in line):
			ref_line_ending = line_num + 1

			# Determining the line with the method name on it can be 
			# a bit tricky bc people put their curly brace's in 
			# different locations. The current solution is to begin at
			# the line with the first opening curly brace. The 'pointer' will
			# then iterating backwards until a line with something is found on it.
			pointer = ref_line_beginning
			while True:

				# Curly brace and carriage return
				if (len(c_file[pointer]) > 3 and '(' in c_file[pointer]):
					break
				else:
					pointer -= 1

			method_line = c_file[pointer].strip()
			# DEBUG line
			#print(f"Method line: {method_line}; Beginning curly: {ref_line_beginning}, File: {c_source}, Length of Line: {len(c_file[pointer])}")

			method_name = parseMethod(method_line)

			csv_line = f"{c_source},{method_name},{ref_line_beginning},{ref_line_ending},{ref_line_ending - ref_line_beginning}\n"
			method_file.write(csv_line)

	return


def getMethods():
	"""
	Gets methods from both source and decompiled code
	"""

	# Gather all source files in the source directory
	print("Gathering Source C Methods")
	source_c_files = subprocess.Popen(['find',argv[1],'-name','*.c'],stdout=subprocess.PIPE).stdout.read().decode('utf-8').split()
	
	# File to write to
	method_source_file = open('method_source.csv','w')
	method_source_file.write("File,Method,Beginning Line Number,End Line Number,Total Lines\n")

	# For every C file, produce method information
	for c_file in source_c_files:
		methodFile(c_file, method_source_file)

	method_source_file.close()

	# Gather all decompiled C source files
	decompile_c_files = subprocess.Popen(['find',argv[2],'-name','*.c'],stdout=subprocess.PIPE).stdout.read().decode('utf-8').split()
	print("Gathering Decompiled C Methods")
	# File to write to 
	method_decompile_file = open('method_decompile.csv','w')
	method_decompile_file.write("File,Method,Beginning Line Number,End Line Number,Total Lines\n")

	# For every decompile C file, produce method information
	for decomp_c_file in decompile_c_files:
		methodFile(decomp_c_file,method_decompile_file)

	method_decompile_file.close()


def cleanUp():
	"""
	Remove all unwanted files that were created during this script.
	"""
	# Files generated by the whole process are appended here
	remove_files = ['source.csv','decomp.csv','method_decompile.csv','method_source.csv']

	for file in remove_files:
		subprocess.Popen(['rm',file])

def parseContext(line):
	comma_split = line.split(",")
	for i in range(len(comma_split) - 1, 0, -1):
		# Iterating backwards through the list until it hits something with CWE
		# Then rebuilds up the the signature
		if ('CWE' in comma_split[i]):
			context = comma_split[i+1:-2]
			context = ",".join(context)
			context = context.strip('"').strip()
			return '"' + context + '"'


def compileCSV(flawfinder_file,method_file,compiled_file):
	"""
	Compile all results from flawfinder_file and method_file into the compiled_file
	"""
	compiled_file.write("File,Method,Beginning Line Number,Final Line Number,Hit Line Number,Name,Context,UID\n")

	# Match every hit to the corresponding method
	for m_line in open(method_file,'r'):

		# Skip header
		if ('File,Method' in m_line):
			continue

		try:
			m_line = m_line.strip().split(',')
			m_file = m_line[0] # Should only have to compare file to file to find the corresponding method
			method_name = m_line[1]
			method_beginning_line = int(m_line[2])
			method_final_line = int(m_line[3])
		except:
			continue

		for f_line in open(flawfinder_file,'r'):
			if ('File,Line' in f_line):
				continue

			context = parseContext(f_line)
			f_line = f_line.strip().split(',')

			try:
				f_file = f_line[0]
				hit_line_number = int(f_line[1])
				hit_name = f_line[5]
				uid = f_line[-1]
			except:
				continue
			
			if (m_file == f_file): # Hit was found in the same file

				# DEBUG
				# print(f"Beginning Line: {method_beginning_line}; Final Line: {method_final_line}; Hit Line: {hit_line_number}; UID: {uid}; File: {f_file}")

				# Check hit line No. and if it falls within the given range, the hit is in that method
				if ((hit_line_number > method_beginning_line) and (hit_line_number < method_final_line)):
					csv_line = f"{m_file},{method_name},{method_beginning_line},{method_final_line},{hit_line_number},{hit_name},{context},{uid}\n"
					compiled_file.write(csv_line)

	compiled_file.close()
	return		


def compileInformation():
	"""
	Compile all the information for source code and decompiled code into one CSV file.
	"""
	# Compile source files
	source_compileCSV = open('source_compiled.csv','w')
	compileCSV('source.csv','method_source.csv',source_compileCSV)
	source_compileCSV.close()

	decompile_compileCSV = open('decomp_compiled.csv','w')
	compileCSV('decomp.csv','method_decompile.csv',decompile_compileCSV)
	decompile_compileCSV.close()

	return

def writeHeader(f):
	header = """
<!DOCTYPE html>
<html>
	<head>
		<title>Results</title>
	</head>
	<body>
		<h1>Results</h1>
			 """

	f.write(header)

def writeFooter(f):
	footer = """
	</body>
</html>
			 """
	f.write(footer)

def writeScannedFiles(html):
	# All source files
	html.write("<h3>Scanned Source Files</h3>\n")
	source_c_files = subprocess.Popen(['find',argv[1],'-name','*.c'],stdout=subprocess.PIPE).stdout.read().decode('utf-8').split()
	for file in source_c_files:
		c_file = file.split('/')[-1]
		html.write(f"	{c_file}<br>\n")

	# All decompiled files
	html.write("<h3>Scanned Decompiled Files</h3>\n")
	decompile_c_files = subprocess.Popen(['find',argv[2],'-name','*.c'],stdout=subprocess.PIPE).stdout.read().decode('utf-8').split()
	for file in decompile_c_files:
		c_file = file.split('/')[-1]
		html.write(f"	{c_file}<br>\n")

def buildAverageStruct():
	"""
	Sort all collected source information into a dictionary
	"""
	global hit_list

	# {method name:{categoory:count,category:count,...}}
	# {method:[[context,hit_line,beg_line,fin_line],...]}

	# {method: {name: {source_context: [{probability:decomp_info},{probability:decomp_info},...] } } }

	# Fill method, category, source_context
	for line in open('source_compiled.csv','r'):
		if ('File,Method' in line):
			continue

		source_method = line.strip().split(',')[1]
		source_beg_line = line.strip().split(',')[2]
		source_fin_line = line.strip().split(',')[3]
		source_hit_line = line.strip().split(',')[4]
		source_name = line.strip().split(',')[5]
		source_uid = line.strip().split(',')[-1]

		source_context = (",".join(line.strip().split(',')[6:-1])).split('\t')[0]
		
		if (source_method not in hit_list.keys()):
			hit_list[source_method] = {}

		if (source_name not in hit_list[source_method].keys()):
			hit_list[source_method][source_name] = {}

		if (source_uid not in hit_list[source_method][source_name].keys()):
			hit_list[source_method][source_name][source_uid] = []
		
		for line in open('decomp_compiled.csv','r'):
			if ('File,Method' in line):
				continue

			decomp_method = line.strip().split(',')[1]
			decomp_beg_line = line.strip().split(',')[2]
			decomp_fin_line = line.strip().split(',')[3]
			decomp_hit_line = line.strip().split(',')[4]
			decomp_name = line.strip().split(',')[5]
			decomp_uid = line.strip().split(',')[-1]

			decomp_context = (",".join(line.strip().split(',')[6:-1])).split('\t')[0]
			decomp_info = {}

			if (decomp_method in hit_list.keys() and decomp_method == source_method):
				if (decomp_name in hit_list[source_method].keys() and decomp_name == source_name):
					info = [decomp_context,decomp_hit_line,decomp_beg_line,decomp_fin_line,decomp_uid]
					decomp_info[100] = info
					hit_list[source_method][source_name][source_uid].append(decomp_info)
			
	if (debug_build):
		for method,name_info in hit_list.items():
			print(f"[+] Method: {method}")
			for name,source_info in name_info.items():
				print(f"\t[+] Name: {name}")
				for source_uid,probability_list in source_info.items():
					print(f"\t\t[+] Source UID: {source_uid}")
					for d in probability_list:
						print(f"\t\t\t[+] {d}")
	
	return

def batchAverage():
	"""
	For each potential corresponding hit, decrease the probability for each corresponding decompile hit
	by the number
	Ex. If you have 4 potential strcpy hits for a method, then the probability of each decompile hit
	    has a 25% chance of being the correct corresponding hit
	"""
	global batch_coefficient
	global hit_list
	global debug_batch

	if (debug_batch):
		for method,name_info in hit_list.items():
			print(f"[+] Method: {method}")
			for name,source_info in name_info.items():
				print(f"\t[+] Name: {name}")
				for source_uid,probability_list in source_info.items():
					print(f"\t\t[+] Source UID: {source_uid}")

					try:
						batch_probability = (100 / len(probability_list)) * batch_coefficient
						batch_probability = round(batch_probability, 2)
					except ZeroDivisionError:
						pass

					
					for i in range(len(probability_list)):

						hit_list[method][name][source_uid][i][batch_probability] = hit_list[method][name][source_uid][i][100]
						
						if (batch_probability != 100):
							del hit_list[method][name][source_uid][i][100]
						
						print(f"\t\t\t[+] Decomp: {hit_list[method][name][source_uid][i]}")
	else: # There must be a better way to do this
		for method,name_info in hit_list.items():
			for name,source_info in name_info.items():
				for source_uid,probability_list in source_info.items():

					try:
						batch_probability = (100 / len(probability_list)) * batch_coefficient
						batch_probability = round(batch_probability, 2)
					except ZeroDivisionError:
						pass

					for i in range(len(probability_list)):

						hit_list[method][name][source_uid][i][batch_probability] = hit_list[method][name][source_uid][i][100]
						
						if (batch_probability != 100):
							del hit_list[method][name][source_uid][i][100]

	return

def buildLocalityStruct():
	# source_line_information = {uid:[hit_line,beginning_line,end_line],uid:[hit_line,beginning_line,end_line]}
	global source_line_information
	global debug_locality

	# Populate the source_line_information
	for line in open('source_compiled.csv','r'):
		if ('File,Method' in line):
			continue

		source_info = []

		source_beg_line = line.strip().split(',')[2]
		source_fin_line = line.strip().split(',')[3]
		source_hit_line = line.strip().split(',')[4]
		source_uid = line.strip().split(',')[-1]

		source_info = [source_hit_line,source_beg_line,source_fin_line]
		if (source_uid not in source_line_information):
			source_line_information[source_uid] = source_info

	if (debug_locality):
		for uid,info in source_line_information.items():
			print(f"[+] UID: {uid}")
			print(f"\t[+] Hit Line: {info[0]}")
			print(f"\t[+] Beginning Line: {info[1]}")
			print(f"\t[+] End Line: {info[2]}")

				
	return

def localityAverage():
	"""
	Bases the probability of a decompiled hit to the source code based on 
	the locality of where the hit was found
	"""
	# source_line_information = {uid:[hit_line,beginning_line,end_line],uid:[hit_line,beginning_line,end_line] }
	global source_line_information
	
	# {method: {name: {source_context: [{probability:decomp_info},{probability:decomp_info},...] } } }
	# decomp_info = [decomp_context,decomp_hit_line,decomp_beg_line,decomp_fin_line,decomp_uid]
	global hit_list

	# Find the corresponding context to every 
	for uid,info in source_line_information.items():

		# Search for line that has the wanted UID
		for line in open('source_compiled.csv','r'):
			if ("File,Method" in line):
				continue

			source_uid = line.strip().split(',')[-1]
			source_method = line.strip().split(',')[1]
			source_name = line.strip().split(',')[5]
			source_context = (",".join(line.strip().split(',')[6:-1])).split('\t')[0]

			if (uid == source_uid):
				# Re-adjust lines in source to make locality simpler
				source_beg_line = int(source_line_information[uid][1])

				source_hit_line = int(source_line_information[uid][0]) - source_beg_line
				source_end_line = int(source_line_information[uid][2]) - source_beg_line
				source_beg_line = source_beg_line - source_beg_line

				print(f"[+] Source: {source_context}")
				print(f"\t\t[+] Adjusted Hit Line: {source_hit_line}")
				print(f"\t\t[+] Adjusted Beginning Line: {source_beg_line}")
				print(f"\t\t[+] Adjusted End Line: {source_end_line}")
				print("")

				# Get corresponding potential hits from decompiler
				for pot_hit in hit_list[source_method][source_name][source_uid]:
					
					for prob,decomp_info in pot_hit.items():
						# Re-adjust lines in decompiler to make locality simpler

						# The -1 here is because the beginning line in all decompiled source code is off by 1.
						# I have no idea why but I'm accounting for hit here. This 'off-by-one' thing is not a big
						# deal but could cause some problems potentially somewhere down the line
						decompile_beg_line = int(decomp_info[2]) - 1 

						decompile_hit_line = int(decomp_info[1]) - decompile_beg_line
						decompile_end_line = int(decomp_info[3]) - decompile_beg_line
						decompile_beg_line = decompile_beg_line - decompile_beg_line

						print(f"\t[+] Decompiled: {decomp_info[0]}")
						print(f"\t\t[+] Adjusted Hit Line: {decompile_hit_line}")
						print(f"\t\t[+] Adjusted Beginning Line: {decompile_beg_line}")
						print(f"\t\t[+] Adjusted End Line: {decompile_end_line}")
						print("")

					
	return

def multiStagedAnalysis():
	"""
	Entry point method for analysis. Remove or append custom methods to determine the 
	probability of every hits relationship
	"""
	## Custome structures to help with determining probability
	buildAverageStruct()
	# buildLocalityStruct()

	## Actual averaging down here
	batchAverage()
	# localityAverage()


def main():
	parseArgs()
	flawfinder()
	getMethods()
	compileInformation()
	multiStagedAnalysis()

	## TODO Allow the user to supply the name of the html file
	html_file = open('results.html','w')

	writeHeader(html_file)
	writeScannedFiles(html_file)
	writeFooter(html_file)

	html_file.close()
	cleanUp()

if __name__ == "__main__":
	main()