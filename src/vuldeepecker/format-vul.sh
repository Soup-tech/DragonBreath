#!/bin/bash
#
# USAGE: ./format-vul.sh [path/to/file]

top_line=$(cat $1 | grep -nx 'INFO  GhidraDecompiler.java> ' | cut -d ':' -f 1)
bottom_line=$(cat $1 | grep -nx ' (GhidraScript)  ' | cut -d ':' -f 1)
$(head -`expr $bottom_line - 1` $1 | tail +`expr $top_line + 1` > $2)