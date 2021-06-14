#!/usr/bin/python3

import sys
import subprocess

def main():
	if (len(sys.argv) < 4):
		usage()
		exit(1)
	fin_list = []
	
	# Store file into list
	for line in open(sys.argv[1],'r'):
		fin_list.append(line.strip())

	# Begin filtering
	if (('-s' in sys.argv) or ('--source' in sys.argv)):
		for line in reversed(fin_list):
			csv = line.split(',')
			
		# Get the source being filtered
		if ('-s' in sys.argv):
			s_index = sys.argv.index('-s') + 1
		elif ('--source' in sys.argv):
			s_index = sys.argv.index('--source') + 1

		if (sys.argv[s_index] != csv[1]):
			fin_list.remove(line)

	if (('-l' in sys.argv) or ('--layer' in sys.argv)):
		for line in reversed(fin_list):
			csv = line.split(',')

			# Get the layer being filtered
			if ('-l' in sys.argv):
				l_index = sys.argv.index('-l') + 1
			elif ('--layer' in sys.argv):
				l_index = sys.argv.index('--layer') + 1

			if (sys.argv[l_index] != csv[2]):
				fin_list.remove(line)

	if (('-c' in sys.argv) or ('--category' in sys.argv)):
		for line in reversed(fin_list):
			csv = line.split(',')

			# Get the category being filtered
			if ('-c' in sys.argv):
				c_index = sys.argv.index('-c') + 1
			elif ('--category' in sys.argv):
				c_index = sys.argv.index('--category') + 1

			if (sys.argv[c_index] != csv[3]):
				fin_list.remove(line)

	if (('-m' in sys.argv) or ('--method' in sys.argv)):
		for line in reversed(fin_list):
			csv = line.split(',')

			# Get the method being filtered
			if ('-m' in sys.argv):
				m_index = sys.argv.index('-m') + 1
			elif ('--method' in sys.argv):
				m_index = sys.argv.index('--method') + 1

			if (sys.argv[m_index] != csv[4]):
				fin_list.remove(line)
	

	if (('-o' in sys.argv) or ('--output' in sys.argv)):
		# Get name of file to write
		if ('-o' in sys.argv):
			o_index = sys.argv.index('-o') + 1
		elif ('--ouput' in sys.argv):
			o_index = sys.argv.index('--output') + 1

		outputFile = open(sys.argv[o_index],'w')

		for line in fin_list:
			outputFile.write(line + '\n')

		outputFile.close()
	else:
		for line in fin_list:
			print(line)
		


def usage():
	print("USAGE: ./analyze.py [path/to/file.csv] [OPTIONS]..[ -s | -l | -c | -m | -o ]\n" +
		  "OPTIONS:\n" + 
		  "\t-s --source\t: Filter By Source\n" +
		  "\t-l --layer\t: Filter By Layer\n" +
		  "\t-c --category\t: Filter By Category\n" +
		  "\t-m --method\t: Filter By Method\n" +
		  "\t-o --output\t: Write to a File")




if __name__ == "__main__":
	main()