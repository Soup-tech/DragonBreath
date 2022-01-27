#!/usr/bin/env python3

import sys
import os
import subprocess
import string
import re

argv = sys.argv

# List of all methods being scanned for (currently). Helps with handling parameters
method_list = ["__strcpy_chk",
			   "__sprintf_chk",
			   "__syslog_chk",
			   "strcpy",
			   "memcpy",
			   "strcat",
			   "gets",
			   "sprintf",
			   "printf",
			   "syslog",
			   "scanf",
			   "fscanf",
			   "getopt",
			   "chown",
			   "chmod",
			   "vfork",
			   "readlink",
			   "tmpfile",
			   "mktemp",
			   "fopen",
			   "umask",
			   "chroot"]

### Structures
# {method: {name: {source_context: [{probability:decomp_info},{probability:decomp_info},...] } } }
hit_list = {}
# source_line_information = {uid:[hit_line,beginning_line,end_line],uid:[hit_line,beginning_line,end_line] }
source_line_information = {}
# source_hit_buffer_sizes = {uid:{parameter_name:size/type,parameter_name:size/type,...}}
source_hit_buffer_sizes = {}
# {directive_name:amount}
known_directives = {}
# decompile_hit_buffer_sizes = {uid:{parameter_name:size/type,paramter_name:size/type,...}}
decompile_hit_buffer_sizes = {}

### Switches
## Display debugging information for the source data
debug_build = False
## Display debugging information for the decompiled data
debug_batch = False
## Display debugging infromation for the locality data structures
debug_locality = False
## Display debugging information for parameter searching
debug_parameter = False
## Display debugging information for parameter average technique
debug_parameter_average = False

### Coefficients
# Weighted average for batching. Default = 1
batch_coefficient = 1
# Weighted average for locality. Default = 1
locality_coefficient = 1
# Weighted avearge for equivalent parameters and sizes. 
name_and_size_coefficient = 0.5
# Weight average for equivalent names. 
name_coefficient = 0.25
# Weighted average for equivelent sizes. 
size_coefficient = 0.25
# Weighted average when nothing is equivalent. 
none_coefficient = 0.15

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
	global debug_parameter
	global debug_parameter_average

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
			elif (argv[debug_index] == "parameter"):
				debug_parameter = True
			elif (argv[debug_index] == "parameter-average"):
				debug_parameter_average = True
		except:
			error("Supply debugging option: 'build', 'batch-average', 'parameter','parameter-average'")

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
	ref_space = 0
	ref_paran = 0
	for count,char in enumerate(line):
		
		# Save the last instance of the space
		if (char == ' '):
			ref_space = count

		# Found the first paranthesis of the method line
		if (char == '(' and ref_space != 0):
			ref_paran = count
			break
	method_name = line[ref_space + 1:ref_paran]
	
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
				line = c_file[pointer].strip()
				
				if ('(' in line):
					break
				else:
					pointer -= 1
					
			method_line = c_file[pointer].strip()
			# DEBUG line
			#print(f"Method line: {method_line}; Beginning curly: {ref_line_beginning}, File: {c_source}, Length of Line: {len(c_file[pointer])}")

			method_name = parseMethod(method_line)

			csv_line = f"{c_source},{method_name},{pointer},{ref_line_ending},{ref_line_ending - ref_line_beginning}\n"
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
	remove_files = ['source.csv','decomp.csv','method_decompile.csv','method_source.csv','decomp_compiled.csv','source_compiled.csv']

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
		<style>
			table {
				border: none;
				width: 100%;
				text-align: left;
			}
			td,th {
				word-wrap: break-word;
				text-align: left;
				padding: 8px;
			}
			.source-context {
				width: 100px;
			}

		</style>
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

	html.write('<br>')
	html.write('<br>')
	return

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

			decomp_file = line.strip().split(',')[0]
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
					info = [100,decomp_context,decomp_hit_line,decomp_beg_line,decomp_fin_line,decomp_uid,decomp_file]
					#decomp_info[100] = info
					hit_list[source_method][source_name][source_uid].append(info)
			
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

					for p in probability_list:
						p[0] = batch_probability
						print(f"\t\t\t[+] Probability: {p[0]}; Context: {p[1]}")
					
	else: # There must be a better way to do this
		for method,name_info in hit_list.items():
			for name,source_info in name_info.items():
				for source_uid,probability_list in source_info.items():

					try:
						batch_probability = (100 / len(probability_list)) * batch_coefficient
						batch_probability = round(batch_probability, 2)
					except ZeroDivisionError:
						pass
					
					for p in probability_list:
						p[0] = batch_probability
						
					
	
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
		source_file = line.strip().split(',')[0]
		source_method = line.strip().split(',')[1]
		source_beg_line = line.strip().split(',')[2]
		source_fin_line = line.strip().split(',')[3]
		source_hit_line = line.strip().split(',')[4]
		source_uid = line.strip().split(',')[-1]
		source_context = (",".join(line.strip().split(',')[6:-1])).split('\t')[0]

		source_info = [source_hit_line,source_beg_line,source_fin_line,source_file,source_method,source_context]
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

def formatParam(unformatted,identifier):
	"""
	Formats unformatted parameter to extract just the name of the variable being used in the parameter
	"""
	acceptable_characters = string.ascii_letters + string.digits
	if (identifier == '*)'):
		p = unformatted.split(identifier)[1]
	else:
		p = unformatted.split(identifier)[0]

	# This will default scan through the parameter from 0->len(p)
	forward_parse = True

	for c in p:
		if (c not in acceptable_characters):
			forward_parse = False

	# Nothing strange, hopefully....
	if (forward_parse):
		i = 0
		while (p[i] != ' ' and p[i] in acceptable_characters and i < len(p) - 1):
			i += 1
		formatted_param = p[0:i + 1]
	else:
		i = len(p) - 1
		beg_line = i
		while (p[i] != ' ' and p[i] in acceptable_characters and i > -1):
			i -= 1
		formatted_param = p[i + 1:beg_line + 1]

	return formatted_param


def extractParameters(context,category):
	global debug_parameter

	param_list = []
	beg_line = 0
	end_line = 0

	context_length = len(context)
	# Start of the vulnerable libc method
	i = context.index(category)
	paran_counter = 0

	# I may do something similar to curly counter which I used to find method names.
	# I thought about a lot of different ways to do this such as using a combination of regular expressions
	# but could never find anything that was consistent enough to account for all cases
	# so instead I'm going to go with a pointer and check each characters
	while i != context_length:

		curr_char = context[i]
		
		if (curr_char == '(' and paran_counter == 0):
			paran_counter += 1
			beg_line = i # Save the start of the parameters
		elif (curr_char == '('):
			paran_counter += 1
		elif (curr_char == ')'):
			paran_counter -= 1

		# End of libc method
		if (paran_counter == 0 and curr_char == ')'):
			end_line = i
			break

		i+=1

	params = context[beg_line + 1:end_line].split(',')

	# Now we have everything inside the parenthesis for the hit, just need the names
	# this will again be tricky because of what exactly is in the parameters.
	# We could have a casting (i.e. (char *)), pointer to a variable in the heap (i.e. heapVar->var)
	# and even a string literal. There's a lot more that I'm probably not remembering but I'll
	# adjust the code as they come

	if (debug_parameter):
		print(f"[+] Original Context: {context}")
		print(f"[+] Category: {category}")
	
	for param in params:
		param = param.strip()
		#print(f"\t[+] Original Param: {param}")
			
		if ('""' in param):
			# Anything insdie string literals should not be considered 
			# so this if-stmt must come first
			param_list.append(param)

		elif ('->' in param):
			formatted_param = formatParam(param,'->')
			param_list.append(formatted_param)

		elif ('.' in param):
			formatted_param = formatParam(param,'.')
			param_list.append(formatted_param)

		elif ('*)' in param):
			formatted_param = formatParam(param,'*)')
			param_list.append(formatted_param)

		elif ('[' in param):
			formatted_param = formatParam(param,'[')
			param_list.append(formatted_param)

		else:
			param_list.append(param.strip())

	if (debug_parameter):
		print(param_list)
		print('')		


	return param_list

def getPreProcessedDirective(directive):
	grepped = subprocess.Popen(['grep','-r',directive,argv[1]],stdout=subprocess.PIPE).stdout.read().decode('utf-8').split('\n')
	
	for grep in grepped:

		if ("#define" in grep and directive in grep):
			directive_index = grep.split().index(directive) + 1
			directive_value = grep.split()[directive_index]

			return directive_value

	return

def determineBufferSize(parameter_line,param,method_name):
	# {directive_name:amount}
	global known_directives
	pot_method_name = ""

	# See if the variable is inside method parameters
	try:
		pattern = re.compile(r'(?<= ).+?(?=\()')
		result = re.search(pattern,parameter_line)
		if (result is None): 
			pot_method_name = ""
		else:
			pot_method_name = result.group()
	except:
		pass 

	# See if I'm dealing with a string literal
	if ('"' in parameter_line):
		try:
			patter = re.compile(r'\"(.*?)\"')
			result = re.search(pattern,parameter_line)
			if (result is not None):
				return "String Literal"
		except:
			pass

	
	# If what I'm looking for is a parameter to a method
	if (pot_method_name == method_name):

		pattern = re.compile(r'\((.*?)\)')
		result = re.search(pattern,parameter_line)
		if (result is not None):
			params = result.group().split(',')

			for p in params:
				if (param in p):
					parameter_line = p
					break

	if ('int' in parameter_line and '*' in parameter_line):
		return 'int *'
	elif ('int' in parameter_line):
		return 'int *'
	elif ('char' in parameter_line and '*' in parameter_line):
		return 'char *'
	elif ('struct' in parameter_line):
		return 'struct'

	if ('char' in parameter_line and '[' in parameter_line):
		pattern = re.compile(r'(?<=\[).+?(?=\])')
		result = re.search(pattern,parameter_line)

		#print(f"Regex Result: {result.group()}")
		try:
			result = int(result.group())
			return result
		except ValueError:
			# Preprocessed directive probably, find the line where the preprocessed directive was initialized
			directive = result.group()

			if (directive in known_directives.keys()):
				return known_directives[directive]
			else:
				if ('+' in directive or '-' in directive or '*' in directive or '/' in directive):
					directive_pattern = re.compile(r'[a-zA-Z_]+')
					directive = re.search(directive_pattern,directive).group()
										

				directive_size = getPreProcessedDirective(directive)
				known_directives[directive] = directive_size
				return directive_size
	elif ('char' in parameter_line):
		return 'char'

	return "Not Found"

def paramSizeAverage():
	"""
	Calculates the likelihood of each potential decompile hit based on whether the 
	buffer for the parameters are the same size in both the source code and the 
	decompiled code
	"""

	# {method: {name: {source_uid: [[probability:decomp_info],[probability:decomp_info],...] } } }
	# decomp_info = [decomp_context,decomp_hit_line,decomp_beg_line,decomp_fin_line,decomp_uid,decompile_file]
	global hit_list

	# source_line_information = {source_uid:[hit_line,beginning_line,end_line,file,size],uid:[hit_line,beginning_line,end_line,file] }
	global source_line_information

	# source_hit_buffer_sizes = {parameter_name:size/type,parameter_name:size/type,...}
	global source_hit_buffer_sizes

	# decompile_hit_buffer_sizes = {parameter_name:size/type,paramter_name:size/type,...}
	global decompile_hit_buffer_sizes

	global debug_parameter_average
	global name_and_size_coefficient
	global name_coefficient

	source_compiled = open('source_compiled.csv','r').readlines()
	decompile_compiled = open('decomp_compiled.csv','r').readlines()

	# First determine the the buffer size in the parameter(s) of the source code
	# Only have to look within the method
	for method,name in hit_list.items():
		for name,source_context in name.items():
			for uid,decomp_hits in source_context.items():

				# Have corresponding uid for both hit_list and source_line_information
				method_start = int(source_line_information[uid][1])
				method_end = int(source_line_information[uid][2])
				source_file = source_line_information[uid][3]
				method_name = source_line_information[uid][4]

				# Get the context for the source hit
				for line in source_compiled:
					source_uid = line.strip().split(',')[-1]

					if (uid == source_uid):
					
						source_context = (",".join(line.strip().split(',')[6:-1])).split('\t')[0]
						source_name = line.strip().split(',')[5]

						# Extract the parameters from the context
						source_param_list = extractParameters(source_context,source_name)
						
						# Scan through the source code to find where the parameters were initialized
						f = open(source_file,'r').readlines()
						for i in range(method_start,method_end):
							
							# Compare every parameter to every line
							# TODO: The way the parameters are formatted/extracted greatly impacts whether or not 
							#       the initialization line is found. 
							for param in source_param_list:

								if ('""' in param): param = param.replace('""','"')
								
								# Line was found, ensure the param doesn't already have an associated initializer
								if (param in f[i] and param not in source_hit_buffer_sizes):

									t = determineBufferSize(f[i],param,method)
									source_hit_buffer_sizes[param] = t

						# Extract the parameters for each decompiled hit and do appropriate calculations
						decompiled_context = ""
						for d in hit_list[method][name][uid]:

							# Extract parameter names from the decompiled context
							decompiled_context = d[1]
							decompiled_param_list = extractParameters(decompiled_context,source_name)
							
							# Associate every parameter with a buffer size
							decompile_file = d[-1]
							decompile_method_start = int(d[3])
							decompile_method_end = int(d[4])
							f = open(decompile_file,'r').readlines()
							
							for i in range(decompile_method_start,decompile_method_end):
								
								# Compare every parameter to every line
								# TODO: The way the parameters are formatted/extracted greatly impacts whether or not 
								#       the initialization line is found. 
								for decomp_param in decompiled_param_list:
									
									if ('""' in decomp_param): decomp_param = decomp_param.replace('""','"')

									# Line was found, ensure param doesn't already have an associated initializer
									if (decomp_param in f[i] and decomp_param not in decompile_hit_buffer_sizes):
										
										t = determineBufferSize(f[i].strip(),decomp_param,method)
										decompile_hit_buffer_sizes[decomp_param] = t

							### This is where the average actually gets averaged. I considred making a method for this but had
							### no way of referencing each source hit to the parameters without saving the uid							
							decompile_hit_probability = d[0]
							for source_parameter_name,source_size in source_hit_buffer_sizes.items():
								i = 0
								for decompile_parameter_name,decompile_size in decompile_hit_buffer_sizes.items():
									
									try:
										source_size = int(source_size)
										decompile_size = int(decompile_size)
									except:
										pass

									
									if (source_parameter_name == decompile_parameter_name and source_size == decompile_size):
										decompile_hit_probability = decompile_hit_probability + (100 * name_and_size_coefficient)
										decompile_hit_buffer_sizes.pop(decompile_parameter_name)
										i += 1
										break
									
									elif (source_parameter_name == decompile_parameter_name):
										decompile_hit_probability = decompile_hit_probability + (100 * name_coefficient)
										decompile_hit_buffer_sizes.pop(decompile_parameter_name)
										i += 1
										break

									elif (source_size == decompile_size and source_size != 'Not Found'):
										decompile_hit_probability = decompile_hit_probability + (100 * size_coefficient)
										decompile_hit_buffer_sizes.pop(decompile_parameter_name)
										i += 1
										break

							# If nothing matched decrease the probability
							if (i == 0):
								decompile_hit_probability = decompile_hit_probability - (100 * none_coefficient)

							
							# Cannot exceed 100%
							if (decompile_hit_probability > 100):
								decompile_hit_probability = 100
							
							d[0] = decompile_hit_probability
							# Clear the dictionaries for the next round of hits
							decompile_hit_buffer_sizes = {}
						source_hit_buffer_sizes = {}
	
	if (debug_parameter_average):
		for method,name in hit_list.items():
			print(f"[+] Method: {method}")
			for name,source_context in name.items():
				print(f"\t[+] Category: {name}")
				for uid,decomp_hits in source_context.items():
					print(f"\t\t[+] UID: {uid}")
					source_context = source_hit_buffer_sizes
					for d in decomp_hits:
						print(f"\t\t\t[+] Decompile Hit: {d}")
					
	return

def writeTable(html):

	# source_line_information = {uid:[hit_line,beginning_line,end_line],uid:[hit_line,beginning_line,end_line] }
	global source_line_information
	# {method: {name: {source_uid: [[probability,decomp_info],[probability,decomp_info],...] } } }
	# decomp_info = [decomp_context,decomp_hit_line,decomp_beg_line,decomp_fin_line,decomp_uid,decompile_file]
	global hit_list

	table_head = """
		<table cellspacing="0" cellpadding="0">
			<tr>
				<th class="source-uid">UID</th>
				<th class="source-context">Source Context</th>
				<th class="decompiler-uid">UID</th>
				<th class="decompiler-context">Decompiler Context</th>
				<th class="decompiler-likelihood">Likelihood</th>
			</tr>
	"""
	html.write(table_head)

	for method,name in hit_list.items():
		for name,source_context in name.items():
			for uid,decomp_hits in source_context.items():
			
				# Gather information to write to the html file
				source_context = source_line_information[uid][-1]

				# Determine the most probable decompiler hit. There could be more than one 'highest' probability hit
				most_likeli_hits = []
				highest_probability_in_list = 0

				# Find the highest probability in the list
				for hit in decomp_hits:
					decompiler_hit_probability = hit[0]
					
					if (decompiler_hit_probability > highest_probability_in_list):
						highest_probability_in_list = decompiler_hit_probability

				for hit in decomp_hits:
					decompiler_hit_probability = hit[0]

					if (decompiler_hit_probability == highest_probability_in_list):
						most_likeli_hits.append(hit)

				likeli_list_length = len(most_likeli_hits)

				source_hit = f"""
					<tr>
						<td>{uid}</td>
						<td>{source_context}</td>
				"""
				html.write(source_hit)

				if (len(most_likeli_hits) == 0):
					no_hit = f"""
						<td></td>
						<td>No corresponding decompiler hit found.</td>
						<td>
					<tr>
					"""
					html.write(no_hit)
				else:
					uid_br  = ""
					decompiler_br = ""
					likelihood_br = ""

					for i in range(len(most_likeli_hits)):
						hit = most_likeli_hits[i]

						uid = hit[5]
						decompiler_context = hit[1]
						likelihood = hit[0]

						uid_br += f"{uid}<br>"
						decompiler_br += f"{decompiler_context}<br>"
						likelihood_br += f"{likelihood}<br>"

					
					decompiler_hit = f"""
						<td>{uid_br}</td>
						<td>{decompiler_br}</td>
						<td>{likelihood}%</td>
					"""

					html.write(decompiler_hit)


	table_foot = """
		</table>
	"""
	html.write(table_foot)
	return

def multiStagedAnalysis():
	"""
	Entry point method for analysis. Remove or append custom methods to determine the 
	probability of every hits relationship
	"""
	## Custome structures to help with determining probability
	buildAverageStruct()
	buildLocalityStruct()

	## Actual averaging down here
	batchAverage()
	#localityAverage()

	paramSizeAverage()


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
	writeTable(html_file)
	writeFooter(html_file)

	html_file.close()
	cleanUp()

if __name__ == "__main__":
	main()