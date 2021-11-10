#!/bin/bash
# echo Encode test	# This is a comment, too!
# for ((c = 0; c < 47; c++))
for c in {0..46}
do
    ./crc imp -c $c
    # echo "loop $c"
done
