#!/bin/sh
#$ -cwd
#$ -l f_node=1
#$ -l h_rt=0:20:00
# Rule to bind core to thread for OMP thread with Intel compiler for parallel version
export KMP_AFFINITY=compact  
export OMP_NUM_THREADS=14     # Set OMP number of threads for parallel version

echo "O0 optimizations : "
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
# ./a.out 16384
# ./a.out 32768
# ./a.out 65536
