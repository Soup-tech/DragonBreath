#!/bin/bash

vuln_soft=$(ls $1)

for dir in $vuln_soft
do
	cd $1'/'$dir
	mkdir cppcheck 2> /dev/null
	cppcheck --enable=all 'src_'$dir --output-file=cppcheck/enable-all.txt
done

exit 0