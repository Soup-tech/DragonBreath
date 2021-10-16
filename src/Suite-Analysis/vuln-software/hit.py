#!/usr/bin/python3

import sys

argv = sys.argv
hits = {}

for line in open(argv[1],'r'):
	line = line.strip().split(',')
	
	project = line[0]
	src = line[1]
	severity = line[2]
	
	if (project not in hits.keys()):
		hits[project] = {}

	if (src not in hits[project].keys()):
		hits[project][src] = {}

	if (severity not in hits[project][src].keys()):
		hits[project][src][severity] = 1
	else:
		hits[project][src][severity] += 1

for proj,src in hits.items():
	print("Project: {}".format(proj))
	for decomp,sev in src.items():
		print("\t{}".format(decomp))
		for num,count in sev.items():
			print("\t\t{}: {}".format(num,count))