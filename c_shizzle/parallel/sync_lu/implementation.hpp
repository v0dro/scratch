#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include "utils.hpp"

void diagonal_block_lu(double *A, int ia, int nb, int N, int *ipiv,
                       int *BLACS_CONTEXT, int *desca);

#endif
