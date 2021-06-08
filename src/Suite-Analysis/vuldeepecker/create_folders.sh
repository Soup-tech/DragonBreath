#!/bin/bash

# ./create_folders.sh [path/to/output/folder] [folder_name]

cd $1
dirs=$(ls)

for d in $dirs:
do
	cd $d
	mkdir $2
	cd ..
done