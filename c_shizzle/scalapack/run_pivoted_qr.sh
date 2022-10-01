#!/bin/bash
mpicxx -I${VEC_LIB_INCLUDE} -L/opt/homebrew/Cellar/scalapack/2.2.0_1/lib -lscalapack -framework Accelerate pivoted_qr.cpp
mpirun -n 1 ./a.out
