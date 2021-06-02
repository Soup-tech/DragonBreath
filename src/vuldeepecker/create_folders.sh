#!/bin/bash

# ./create_folders.sh [path/to/VulDeePacker/CWE-###/source_files/]

cd $1
folder=$(ls)

for dir in $folder
do
	cd $dir
	echo Working $dir
	mkdir src Ghidra 2> /dev/null
	mv *.cpp src 2> /dev/null
	mv *.c src 2> /dev/null
	cd ..
done
