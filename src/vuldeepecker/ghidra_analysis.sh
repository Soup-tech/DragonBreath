#!/bin/bash

# ./ghidra_analysis [path/to/source]

cd $1
dirs=$(ls)

for d in $dirs
do
	echo Running analysis on $d...
	cd $d
	mkdir Ghidra/dragonBreathOutput 2> /dev/null
	ex=$(find . -type f -executable | cut -d '/' -f 2)
	objdump -t $ex | grep .text > Ghidra/$ex.text
	cd ..
done