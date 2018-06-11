#!/bin/bash
export KMP_AFFINITY=compact  #Rule to bind core to thread for OMP thread with Intel compiler for parallel version
export OMP_NUM_THREADS=8     #Set OMP number of threads for parallel version
make clean
make -k main

./a.out 16
./a.out 32
./a.out 64
./a.out 128
./a.out 256
./a.out 512
./a.out 1024
./a.out 2048
./a.out 4096
./a.out 8192
