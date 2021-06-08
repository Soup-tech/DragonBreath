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

	csv_list = findFiles()

	for csv in csv_list:
		csv_lst = csv.split('/')

		# ProjectName
		csv_id = csv_lst[-4]
		# Source
		csv_src = csv_lst[-3]
		
		for line in open(csv,'r'):
			if (line[:4] == 'File'):
				continue
			
			line = line.split(',')
			layer = line[3]
			category = line[4]
			method = line[5]
			context = line[-2]

			print(line)
			results.write(csv_id + "," + csv_src + "," + layer + "," + category + "," + method + "," + context + "\n")
			

	results.close()




def findFiles():
	proc = subprocess.Popen("find " + sys.argv[1] + " -type f -name 'results.csv'", shell=True, stdout=subprocess.PIPE)
	csv_files = proc.stdout.read().decode('utf-8')
	csv_files = csv_files.split('\n')

	return csv_files


def usage():
	print("USAGE: ./analytics.py [path/to/output]")


if __name__ == "__main__":
	main()