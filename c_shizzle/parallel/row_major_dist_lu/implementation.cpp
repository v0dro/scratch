#include "implementation.hpp"

// block - global block number
// i - global column number within panel
// lvmax [out] - local max element
// limax [out] - local max index
void find_local_max_element(double *A, int block, int i, desc desc_a,
                            double &lvmax, int &limax, int num_procs)
{ 
  int global_index = (block+i)*desc_a.N + (block+i), local_index;
  global2local(global_index, &local_index, num_procs, desc_a);
  lvmax = A[local_index];
  limax = local_index;
  for (int row = block+i; row < desc_a.N; row += 1) {
    local_index += desc_a.lld;
    if (lvmax < A[local_index]) {
      lvmax = A[local_index];
      limax = local_index;
    }
  }
}

// Find the max element in a column given the global block and column number.
//
// @param *imax [out] index in global array with max element.
// @param *vmax [out] max element in the global array.
void find_max_element_in_col(double *A, int block, int i, int * imax,
                             double * vmax, desc desc_a, int myrow, int mycol,
                             int num_procs)
{
  // convert global index to local index
  double lvmax; int limax;
  find_local_max_element(A, block, i, desc_a, lvmax, limax, num_procs);
  cout << "myrow " << myrow << " mycol " << mycol << " lvmax " << (lvmax) << endl;
  // broadcast local max indexes and numbers along the process columns.
  // choose max element and corresponding global index among broadcasted numbers.
}

void pivot_column(double *A, int block, int nb, desc desc_a, int myrow,
                  int mycol, int num_procs)
{
  // iterate over columns witin this vertical panel.
  for (int i = 0; i < nb; ++i) {
    double vmax; int imax; // imax - max index. vmax - max element.
    find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, myrow, mycol, num_procs);
  }
}

// Calculate the LU decomposition.
//
// Each block of the matrix is subdivided among all processes in a block cyclic manner.
//   The data distribution format is the same as that used in scalapack. The 'unit of computation'
//   is the matrix block of size MB * NB, which belongs to a global matrix of size M * N.
//
// The implementation is a right-looking block LU decomposition.
void diagonal_block_lu(double *A, int *ipiv, int *desca, desc desc_a,
                       int myrow, int mycol, int num_procs)
{
  if (COL_MAJOR) {
    int info;
    int ia = 1;
    pdgetrf_(&desc_a.M, &desc_a.N, A, &ia, &ia, desca, ipiv, &info);
  }
  else if (ROW_MAJOR) {
    for (int block = 0; block < desc_a.N; block += desc_a.NB) {
      pivot_column(A, block, desc_a.NB, desc_a, myrow, mycol, num_procs);
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
