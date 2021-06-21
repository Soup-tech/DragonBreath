#!/bin/bash

# ./itc_src_flawfinder.sh [path/to/itc]

cd $1
files=$(ls)

for f in $files
do
	cd $f

	# flawfinder for src
	cd src
	echo + Working on src for $f...
	flawfinder --context --columns --html src_$f > flawfinder/results.html
	flawfinder --csv src_$f > flawfinder/results.csv
	cd ..

	# flawfinder for retdec
	cd retdec
	flawfinder --context --columns --html src_retdec/$f > flawfinder/results.html
	flawfinder --csv src_retdec/$f > flawfinder/results.csv
	cd ..

	# flawfinder for Ghidra
	cd Ghidra
	
	cd ..
	cd ..
done