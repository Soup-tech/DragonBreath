#!/bin/bash

# ./create_folders.sh [path/to/itc]

cd $1
folder=$(ls)

for f in $folder
do
	cd $f
	echo Working on $f

	# Get name of folder
	name=$(pwd | cut -d '/' -f 8)
	
	# Setting up top directory
	mkdir src Ghidra retdec

	# Setting up src
	cd src && mkdir src_$name flawfinder && cd ..
	mv *.c src/src_$name
	# Setting up Ghidra
	cd Ghidra && mkdir flawfinder && cd ..
	# Setting up retdec
	cd retdec && mkdir flawfinder src_retdec && cd ..

	cd ..
done