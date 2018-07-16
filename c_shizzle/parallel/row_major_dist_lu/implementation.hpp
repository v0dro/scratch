#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include "utils.hpp"

#define TAG_0 0
#define TAG_1 1
#define TAG_2 2

void diagonal_block_lu(double *A, int *ipiv, desc desc_a, mpi_desc mpi);
  
#endif
