#include "implementation.hpp"

// Convert local co-ordinate per process to global matrix co-ordinate.
//   Works on flat arrays only.
//
// @param *local [in] local array location.
// @param *global [out] global array location.
void local2global(int *local, int *global, int myrow, int mycol)
{
  
}

// Convert global co-ordinate per process to local matrix co-ordinate.
//   Works on flat arrays only.
//
// @param *global [in] global array location.
// @param *local [out] local array location.
void global2local(int *global, int *local, int myrow, int mycol)
{
  
}

// Find the max element in a column given the global block and column number.
//
// @param *imax [out] index in global array with max element.
// @param *vmax [out] max element in the global array.
void find_max_element_in_col(double *A, int block, int i, double * imax,
                             double * vmax, desc desc_a)
{
  // find local max in each array along with respective local index.
  double max = A;
  // convert local index to global index.
  // broadcast local max indexes and numbers along the process columns.
  // choose max element and corresponding global index among broadcasted numbers.
}

void pivot_column(double *A, int block, int ia, int nb, desc desc_a)
{
  // iterate over columns witin this vertical panel.
  for (int i = 0; i < nb; ++i) {
    double imax, vmax; // imax - max index. vmax - max element.
    find_max_element_in_col(A, block, i, &imax, &vmax, desc_a);
  }
}

// Calculate the LU decomposition.
//
// Each block of the matrix is subdivided among all processes in a block cyclic manner.
//   The data distribution format is the same as that used in scalapack. The 'unit of computation'
//   is the matrix block of size MB * NB, which belongs to a global matrix of size M * N.
//
// The implementation is a right-looking block LU decomposition.
void diagonal_block_lu(double *A, int *ipiv, int *desca, desc desc_a)
{
  if (COL_MAJOR) {
    int info;
    ia = 1;
    pdgetrf_(&desc_a.M, &desc_a.N, A, &ia, &ia, desca, ipiv, &info);
  }
  else if (ROW_MAJOR) {
    for (int block = 0; ia < N; ia += nb) {
      pivot_column(A, block, ia, nb, desc_a);
      // swap_panels();
      // update_upper_panel();
      // update_trailing_submatrix();
      
      // perfrom pivoting of a column
      // update panels to the right and left of pivoted column
      // do a trsm to update the upper panel
      // update the trailing matrix
    }
  }

  // compute LU of diagonal block.
  // broadcast block along rows and cols.
  // broadcast row and col blocks along the lower right block of the matrix and multiply.
}
