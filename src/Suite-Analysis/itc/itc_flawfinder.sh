#!/bin/bash
#
# ./itc_flawfinder.sh [path/to/itc]

cd $1
folders=$(ls)

for f in $folders
do
	cd $f
	cd src

	echo + Working on $f
	flawfinder --context --columns --html src_$f > flawfinder/results.html 2> /dev/null
	flawfinder --csv src_$f > flawfinder/results.csv 2> /dev/null

	cd ..
done