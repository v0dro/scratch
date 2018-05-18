#include "implementation.hpp"

// Calculate the LU decomposition of a single block. The result is stored in the
// A array itself, and pivoting information in ipiv. ipiv stores the pivoting for
// global array of nrows N.
void diagonal_block_lu(double *A, int ia, int nb, int N, int *ipiv,
                       int *BLACS_CONTEXT, int *desca)
{
  int info;
  ia = 1;
  nb = 8;
  pdgetrf_(&nb, &nb, A, &ia, &ia, desca, ipiv, &info);
}
