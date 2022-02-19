#!/bin/bash

FAILEDINDEX=0

if [ $# -eq 0 ]
then
    RANGE_LOW=8
    RANGE_HIGH=10
else
    RANGE_LOW=$1
    RANGE_HIGH=$2
fi
ARG3=$3

for (( i = $RANGE_LOW; i <= $RANGE_HIGH; i++ ))
do
    echo "Testing size: $i"
    
    if [[ $# -gt 2 && $ARG3 = "faroch" ]]
    then
        make faroch SIZE=$i --no-print-directory
    else
        make SIZE=$i --no-print-directory
    fi

    if [ $? -gt 0 ] 
    then
        FAILED[$FAILEDINDEX]=$i
        FAILEDINDEX=$((FAILEDINDEX+1))
    fi
done

echo
echo "Tested sizes $RANGE_LOW to $RANGE_HIGH inclusive."
if [ $FAILEDINDEX -gt 0 ] 
then
    tput bold
    if [ $FAILEDINDEX -gt 1 ]
    then
        echo "Failed for sizes:"
    else
        echo "Failed for size:"
    fi
    echo ${FAILED[@]}
    tput sgr0
else
    echo "All OK"
fi
