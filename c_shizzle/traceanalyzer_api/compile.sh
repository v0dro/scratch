#!/bin/bash

source ~/.bashrc

module purge
module load intel-mpi/2021.5 gcc intel-mkl/2022.0.0 cmake/3.22.3 intel-vtune intel-itac

source /apps/intel/2022.1/itac/2021.5.0/env/vars.sh

mpicxx main.cpp
