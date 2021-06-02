#!/bin/bash
#
# USAGE ./compile.sh [path/to/VulDeePacker/CWE-###/source_files/]

cd $1
folder=$(ls)

for dir in $folder
do
	cd $dir'/src'
	c_files=$(ls *.c 2> /dev/null)
	cpp_files=$(ls *.cpp 2> /dev/null)
	
	if [ $(cat $c_files | wc -l) -gt 0 ]; then
		echo Compiling $dir
		for c in $c_files
		do
			gcc $c -o ../$c.out 2> /dev/null  
		done
	elif [ $(cat $cpp_files | wc -l) -gt 0 ]; then
		echo Compiling $dir
		for cpp in $cpp_files
		do
			g++ $cpp_files -o ../$cpp.out 2> /dev/null 
		done
	fi
	
	cd ..
done
