#!/usr/bin/python3

import subprocess
import sys

def main():
	if (len(sys.argv) < 2):
		usage()
		exit(1)

	proc = subprocess.Popen("find " + sys.argv[1] + " -type f -name 'results.csv'",shell=True,stdout=subprocess.PIPE)
	csv_files = proc.stdout.read().decode('utf-8')
	csv_files = csv_files.split('\n')

	proj_name = projectName()
	
	outputFile = open(sys.argv[1]+proj_name+'.csv','w')
	
	for csv in csv_files:
		if (not csv): # Empty string
			continue
		
		source = csv.split('/')
		source = source[-3]		

		for line in open(csv,'r'):
			if (line[:4] == "File"):
				continue

			line = line.split(',')

			layer = line[3]
			category = line[4]
			method = line[5]
			
			outputFile.write(proj_name + "," + source + "," + layer + "," + category + "," + method + '\n')

	outputFile.close()

def projectName():
	proj_name = sys.argv[1].split('/')
	proj_name = proj_name[-2]
	return proj_name
	 


def usage():
	print("USAGE: ./results.py [path/to/completed/directory]")




if __name__ == "__main__":
	main()