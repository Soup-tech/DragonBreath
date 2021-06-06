#!/bin/bash

# USAGE: ./retdec_analysis.sh [path/to/output]

cd $1
dirs=$(ls)

for d in $dirs
do
	echo " + Analyzing $d"
	cd $d
	mkdir retdec && cd retdec && mkdir src_retdec flawfinder && cd .. 2> /dev/null
	ex=$(find . -type f -executable | cut -d '/' -f 2)

	# retdec analysis
	echo "		+ Running retdec on $ex"
	/opt/retdec/bin/retdec-decompiler.py $ex -o retdec/src_retdec/$ex > /dev/null

	# run flawfinder
	echo "		+ Running flawfinder on $ex"
	flawfinder --context --columns --html retdec/src_retdec/$ex > retdec/flawfinder/results.html
	flawfinder --csv retdec/src_retdec/$ex > retdec/flawfinder/results.csv

	cd ..
done