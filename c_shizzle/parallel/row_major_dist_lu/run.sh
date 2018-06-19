#!/bin/bash

rm *.txt
make clean
make -j8 -k sync_lu
python remake_matrix.py col

