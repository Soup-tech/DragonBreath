#!/bin/bash

# ./itc_src_flawfinder.sh [path/to/itc]

cd $1
files=$(ls)

for f in $files
do
	cd $f
	echo -e " + Working on $f..."

	# flawfinder for src
	echo -e "\t+ src"
	cd src
	flawfinder --context --columns --html src_$f > flawfinder/results.html
	flawfinder --csv src_$f > flawfinder/results.csv
	cd ..

	# flawfinder for retdec
	echo -e "\t+ retdec"
	cd retdec
	flawfinder --context --columns --html src_retdec/$f > flawfinder/results.html
	flawfinder --csv src_retdec/$f > flawfinder/results.csv
	cd ..

	# flawfinder for Ghidra
	echo -e "\t+ Ghidra"
	cd Ghidra
	flawfinder --context --columns --html src_Ghidra > flawfinder/results.html
	flawfinder --csv src_Ghidra > flawfinder/results.csv
	cd ..
	
	cd ..
done