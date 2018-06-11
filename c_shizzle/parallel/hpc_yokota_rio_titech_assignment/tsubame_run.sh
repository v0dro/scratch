#!/bin/bash

make clean
make -k main
qsub -g tga-hpc-lecture job.sh
