#include "implementation.hpp"

// Calculate the LU decomposition.
//
// Each block of the matrix is subdivided among all processes in a block cyclic manner.
//   The data distribution format is the same as that used in scalapack. The 'unit of computation'
//   is the matrix block of size MB * NB, which belongs to a global matrix of size M * N.
//
// The implementation is a right-looking block LU decomposition.
void diagonal_block_lu(double *A, int ia, int nb, int N, int *ipiv,
                       int *BLACS_CONTEXT, int *desca)
{
  if (COL_MAJOR) {
    int info;
    ia = 1;
    nb = 8;
    pdgetrf_(&nb, &nb, A, &ia, &ia, desca, ipiv, &info);
  }
  else if (ROW_MAJOR) {
    
    for (int ia = 0; ia < N; ia += nb) {
      // get the largest element in a row
      // The is
    }
  }

  // compute LU of diagonal block.
  // broadcast block along rows and cols.
  // broadcast row and col blocks along the lower right block of the matrix and multiply.
}
