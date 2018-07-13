#!/bin/bash

if [ "$(whoami)" != "17M38101" ]; then
    rm *.txt
    make clean
    make -j8 -k sync_lu
    python remake_matrix.py $1
else
    echo "Running in TSUBAME debug mode..." 
    rm *.txt
    make clean
    make -j8 -k sync_lu_tsubame
    python remake_matrix.py $1   
fi

