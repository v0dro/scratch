#!/bin/bash

cd /home/sameer/gitrepos/scalapack-2.0.2
make clean
make -j100 lib
cd /home/sameer/gitrepos/scratch/c_shizzle/parallel/sync_lu
rm *.txt
make clean
make -j8 -k sync_lu
python remake_matrix.py col

