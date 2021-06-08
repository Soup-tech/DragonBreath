#!/bin/bash
#
# ./flawfinder-analysis [path/to/output]

cd $1
folder=$(ls)

for dir in $folder
do
	cd $dir/Ghidra

	mkdir flawfinder 2> /dev/null
	echo Running flawfinder on $dir
	flawfinder --context --columns --html src_Ghidra > flawfinder/results.html
	flawfinder --csv src_Ghidra > flawfinder/results.csv
	
	cd ../../
done