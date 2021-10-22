#!/usr/bin/python3

import sys

argv = sys.argv
hits = {}
total = {}

def main():

	if ('-i' in argv):
		byIndividual()
	elif ('-s' in argv):
		bySum()
	else:
		print("Specifiy -i (Indiviual count) or -s (Sum count)")
		sys.exit(-1)

	sys.exit(0)


def bySum():
	for line in open(argv[1],'r'):
		line = line.strip().split(',')

		src = line[1]
		category = line[3]

		if (src not in total.keys()):
			total[src] = {}

		if (category not in total[src].keys()):
			total[src][category] = 1
		else:
			total[src][category] += 1

	for src,lst in total.items():
		print("{}".format(src))
		for cat,num in lst.items():
			print("\t{} : {}".format(cat,num))



def byIndividual():

	# Fill dictionary
	for line in open(argv[1],'r'):
		line = line.strip().split(',')

		project = line[0]
		src = line[1]
		category = line[3]

		if (project not in hits.keys()):
			hits[project] = {}

		if (src not in hits[project].keys()):
			hits[project][src] = {}

		if (category not in hits[project][src].keys()):
			hits[project][src][category] = 1
		else:
			hits[project][src][category] += 1


	# Print out in nice format
	for proj,src in hits.items():
		print("Project: {}".format(proj))
		for decomp,sev in src.items():
			print("\t{}".format(decomp))
			for num,count in sev.items():
				print("\t\t{}: {}".format(num,count))

for proj,src in hits.items():
	for decomp,cat in src.items():
		for num,count in cat.items():

			if (decomp not in total.keys()):
				total[decomp] = {}

			if (num not in total[decomp].keys()):
				total[decomp][num] = count
			else:
				total[decomp][num] += count


if __name__ == "__main__":
	main()