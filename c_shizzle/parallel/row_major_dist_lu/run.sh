#!/bin/bash

make clean
make -j8 -k sync_lu
python remake_matrix.py row

