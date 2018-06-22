#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include "utils.hpp"

void diagonal_block_lu(double *A, int *ipiv, int *desca, desc desc_a,
                       int myrow, int mycol, int num_procs);
  
#endif
