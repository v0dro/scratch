#!/bin/bash
#$ -cwd
#$ -l rt_F=4
#$ -l h_rt=12:00:00
#$ -N HATRIX
#$ -o HATRIX_out.log
#$ -e HATRIX_err.log

source ~/.bashrc

module purge
module load intel-mpi/2021.5 gcc/11.2.0 intel-mkl/2022.0.0 cmake/3.22.3

g++ -m64  -I"${MKLROOT}/include" -DUSE_MKL \
    ${MKLROOT}/lib/intel64/libmkl_scalapack_lp64.a \
    -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a \
    ${MKLROOT}/lib/intel64/libmkl_gnu_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a \
    ${MKLROOT}/lib/intel64/libmkl_blacs_intelmpi_lp64.a -Wl,--end-group -lgomp -lpthread -lm -ldl \
    pdgemm_partial.cpp
