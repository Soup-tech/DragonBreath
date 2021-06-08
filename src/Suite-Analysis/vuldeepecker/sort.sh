#!/bin/bash

# ./sort.sh [path/to/output]

cd $1
dirs=$(ls)

for d in $dirs
do
	cd $d
	mv *.c *.cpp src 2> /dev/null
	cd ..
done