#!/bin/bash
#
# USAGE ./compile.sh [path/to/VulDeePacker/CWE-###/source_files/]

cd $1
dir=$(ls) 

for files in $dir
do 
	cd $files
	
	c_files=$(ls *.c 2> /dev/null)
	cpp_files=$(ls *.cpp 2> /dev/null)

	if [ $(echo $c_files | wc -l) -gt 0 ]; then
		echo Compiling $files...
		for c in $c_files
		do
			c_name=$(echo $c | cut -d '.' -f 1 2> /dev/null)
			gcc $c -o $c_name 2> /dev/null
		done
	elif [ $(echo $cpp_files | wc -l) -gt 0 ]; then
		echo Compiling $files...
		for cpp in $cpp_files
		do
			cpp_name=$(echo $cpp | cut -d '.' -f 1 2> /dev/null)
			g++ $cpp -o $cpp_name 2> /dev/null
		done
	fi

	cd ..
done