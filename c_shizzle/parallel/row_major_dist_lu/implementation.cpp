#include "implementation.hpp"

// block - global block number
// i - global column number within panel
// lvmax [out] - local max element
// limax [out] - local max index
void find_local_max_element(double *A, int block, int i, desc desc_a,
                            double &lvmax, double &limax, int num_procs)
{ 
  int global_index = (block+i)*desc_a.N + (block+i); int local_index;
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
void find_max_element_in_col(double *A, int block, int i, double * imax,
                             double * vmax, desc desc_a, mpi_desc mpi)
{
  // convert global index to local index
  double lvmax, limax;
  find_local_max_element(A, block, i, desc_a, lvmax, limax, mpi.num_procs);
  // broadcast local max indexes and numbers along the process columns.
  double max[mpi.MP*2];
  int temp_imax;
  local2global(limax, &temp_imax, mpi.myrow, mpi.mycol, mpi.num_procs, desc_a);
  max[mpi.myrow*mpi.MP]     = lvmax;
  max[mpi.myrow*mpi.MP + 1] = temp_imax;
  // Send the local imax and vmax to all processes in the same column:
  for (int r = 0; r < mpi.MP; r++) {
    Cdgesd2d(mpi.BLACS_CONTEXT, 2, 1, &max[mpi.myrow*mpi.MP], 2, r, mpi.mycol);
  }
  // Receive the imax and vmax of all processes in the same column:
  for (int r = 0; r < mpi.MP; r++) {
    Cdgerv2d(mpi.BLACS_CONTEXT, 2, 1, &max[r*mpi.MP], 2, r, mpi.mycol);
  }
  // choose max element and corresponding global index among broadcasted numbers.
  *vmax = max[0]; *imax = max[1];
  for (int i = 2; i < mpi.MP*2; i += 2) {
    if (*vmax < max[i]) {
      *vmax = max[i];
      *imax = max[i+1];
    }
  }
}

// Swap the rows within this current panel.
void swap_within_current_panel(double *A, desc desc_a, int goriginal, int gnew)
{
  int gorig_row, gorig_col;
  index2coords(goriginal, desc_a.N, gorig_row, gorig_col);
  
}

void pivot_column(double *A, int block, int nb, desc desc_a, mpi_desc mpi)
{
  // iterate over columns witin this vertical panel.
  int curr_global;
  double vmax, imax; // imax - max index. vmax - max element.
  int imyrow, imycol;
  procg2l(block, block, &imyrow, &imycol, desc_a, mpi);

  if (imycol == mpi.mycol) {
    for (int i = 0; i < nb; ++i) {
      curr_global = (block + i)*desc_a.N + block + i;
      find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, mpi); // idamax
      if (imax != curr_global) {
        swap_within_current_panel(A, desc_a, curr_global, imax); // dswap
      }
    }
  }
}

// Calculate the LU decomposition.
//
// Each block of the matrix is subdivided among all processes in a block cyclic manner.
//   The data distribution format is the same as that used in scalapack. The 'unit of computation'
//   is the matrix block of size MB * NB, which belongs to a global matrix of size M * N.
//
// The implementation is a right-looking block LU decomposition.
void diagonal_block_lu(double *A, int *ipiv, int *desca, desc desc_a, mpi_desc mpi)
{
  if (COL_MAJOR) {
    int info;
    int ia = 1;
    pdgetrf_(&desc_a.M, &desc_a.N, A, &ia, &ia, desca, ipiv, &info);
  }
  else if (ROW_MAJOR) {
    for (int block = 0; block < desc_a.N; block += desc_a.NB) {
      pivot_column(A, block, desc_a.NB, desc_a, mpi);
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
