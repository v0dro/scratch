#/bin/bash

source ~/.bashrc

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/sameer/anaconda3/lib
g++ -m64 -I${MKLROOT}/include  -L${MKLROOT}/lib -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl  mkl-sparse.cpp 
