#!/bin/bash

# ./itc_retdec.sh [path/to/itc]

cd $1
folders=$(ls)

for f in $folders
do
	cd $f
	
	echo + Running retdec on $f
	/opt/retdec/bin/retdec-decompiler.py $f -o retdec/src_retdec/$f 2> /dev/null > /dev/null

	cd ..

done