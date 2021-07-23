#!/bin/bash

vuln_soft=$(ls $1)

for dir in $vuln_soft
do
	cd $1'/'$dir
	mkdir flawfinder 2> /dev/null
	flawfinder --context --columns --html 'src_'$dir > flawfinder/results.hmtl
done

exit 0