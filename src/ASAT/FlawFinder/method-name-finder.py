#!/usr/bin/env python3

# CSV file which tracks all method names
method_file = open('method_file.csv','w')
method_file.write('Method,Beginning Line Number,End Line Number,Total Number of Lines\n')

def parseMethod(line):
	"""
	Parse method lines to get just the name of the method
	"""
	method_name = line.split('(')[0].split()[1]
	return method_name

def main():

	# Counter variables
	curly_counter = 0
	ref_line_beginning = 0
	ref_line_end = 0

	# open C source file
	c_file = open('aeon.c','r').readlines()

	for num,line in enumerate(c_file):
		line = line.strip()
		
		# First encounter of a curly brace. The line above is the method name
		if ('{' in line and curly_counter == 0):
			ref_line_beginning = num
			curly_counter = 1

		# Encountered another opening curly brace except we are within a method
		elif ('{' in line):
			curly_counter += 1

		# Encountered a closing curly brace
		elif ('}' in line):
			curly_counter -= 1

		# If curly brace counter equals 0, we have reached the end of the method
		if (curly_counter == 0 and '}' in line):
			# Reference the last curly brace
			ref_line_end = num + 1

			# Get the line with the method name on it. Because people code differently
			# you can have either: 
			#	return_type method_name() { 	OR
			# 	return_type method_name()
			#	{
			# This way of determining the method name really only works if I assume people
			# code in one of two ways.

			# Just the curly and newline
			if (len(c_file[ref_line_beginning]) == 2):
				method_line = c_file[ref_line_beginning - 1]
			else:
				method_line = c_file[ref_line_beginning]
				ref_line_beginning = ref_line_beginning + 1

			method_name = parseMethod(method_line.strip())
			
			csv_line = f"{method_name},{ref_line_beginning},{ref_line_end},{ref_line_end - ref_line_beginning}\n"
			method_file.write(csv_line)

	method_file.close()
	return

if __name__ == "__main__":
	main()	