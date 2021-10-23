#!/bin/bash

cd /home/hades/Downloads/itc_mirkwood/000/199
dirs=$(ls)

remove=$(find . -name results.csv)
for r in $remove
do
	echo -e "Removing $r"
	rm $r
done

for d in $dirs
do
	echo -e "[+] Working on $d"
	cd $d

	echo -e "\t[+] Source"
	cd src
	flawfinder --context --columns --html src_$d > smaug/results.html
	flawfinder --csv src_$d | tee smaug/results.csv -a /home/hades/Downloads/itc_mirkwood/000/godfile.csv > /dev/null
	cd ..

	echo -e "\t[+] Ghidra"
	cd Ghidra
	flawfinder --context --columns --html src_Ghidra > smaug/results.html
	flawfinder --csv src_Ghidra | tee smaug/results.csv -a /home/hades/Downloads/itc_mirkwood/000/godfile.csv > /dev/null
	cd ..

	echo -e "\t[+] RetDec"
	cd retdec
	flawfinder --context --columns --html src_retdec > smaug/results.html
	flawfinder --csv src_retdec | tee smaug/results.csv -a /home/hades/Downloads/itc_mirkwood/000/godfile.csv > /dev/null
	cd ..

	echo -e "\t[+] JEB"
	cd JEB
	flawfinder --context --columns --html src_JEB > smaug/results.html
	flawfinder --csv src_JEB | tee smaug/results.csv -a /home/hades/Downloads/itc_mirkwood/000/godfile.csv > /dev/null
	cd ..

	echo -e "\t[+] IDA"
	cd IDA
	flawfinder --context --columns --html src_IDA > smaug/results.html
	flawfinder --csv src_IDA | tee smaug/results.csv -a /home/hades/Downloads/itc_mirkwood/000/godfile.csv > /dev/null
	cd ..

	cd ..
done