#!/bin/bash
#
# ./flawfinder-analysis [path/to/output]

cd $1
folder=$(ls)

for dir in $folder
do
	cd $dir/src

	mkdir flawfinder 2> /dev/null
	echo Running flawfinder on $dir
	flawfinder --context --columns --html . > flawfinder/results.html
	flawfinder --csv . > flawfinder/results.csv
	
	cd ../..
done