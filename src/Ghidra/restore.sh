#!/bin/bash
#
# USAGE: ./restore.sh [path/to/.text] [method_name]

$(cp $1 text.bak)
$(rm $1)
line=$(grep -no $2 text.bak | cut -d ':' -f 1)
$(tail +$line text.bak > $1)
$(rm text.bak)
