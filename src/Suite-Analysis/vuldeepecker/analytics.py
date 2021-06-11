#!/usr/bin/python3
# projectid, source (Ghidra,src,retdec), Layer, Category, Method, Code
import subprocess
import sys

def main():

	if (len(sys.argv) < 2):
		usage()
		exit(1)

	
	csv_id = ''
	results = open('results.csv','w')
	results.write("ProjectID,Source,Layer,Category,Method,Code\n")
	csv_list = findFiles()

	for csv in csv_list:
		if (not csv):
			continue

		csv_lst = csv.split('/')
		# ProjectName
		csv_id = csv_lst[-4]
		# Source
		csv_src = csv_lst[-3]
		
		print(" + Running on " + csv_id)
		for line in open(csv,'r'):
			if (line[:4] == 'File'):
				continue
			
			context = parseContext(line)
			if (context == None):
				context = ''
			
			line = line.split(',')

			layer = line[3]
			category = line[4]
			method = line[5]

			results.write(csv_id + "," + csv_src + "," + layer + "," + category + "," + method + "," + context + "\n")
			

	results.close()

def parseContext(line):
	comma_split = line.split(",")

	for i in range(len(comma_split) - 1, 0, -1):
		 # Iterating backwards through the list until it hits something with CWE
		 # Then rebuilds up the the signature
		if ('CWE' in comma_split[i]):
			context = comma_split[i+1:-1]
			context = ",".join(context)
			context = context.strip('"').strip()
			return context


def findFiles():
	proc = subprocess.Popen("find " + sys.argv[1] + " -type f -name 'results.csv'", shell=True, stdout=subprocess.PIPE)
	csv_files = proc.stdout.read().decode('utf-8')
	csv_files = csv_files.split('\n')

	return csv_files


def usage():
	print("USAGE: ./analytics.py [path/to/output]")


if __name__ == "__main__":
	main()