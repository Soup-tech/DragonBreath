#!/bin/bash
#
# ./itc_flawfinder.sh [path/to/itc]

cd $1
folders=$(ls)

for f in $folders
do
	cd $f/src/

	echo + Working on $f...
	flawfinder --context --columns --csv src_$f > flawfinder/results.html
	flawfinder --csv src_$f > flawfinder/results.csv

	cd ../..
done