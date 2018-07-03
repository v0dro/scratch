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
void swap_within_current_panel(double *A, desc desc_a, mpi_desc mpi,
                               int goriginal, int gnew, int block)
{
  // get original global row and col
  int go_row, go_col;
  index2coords(goriginal, desc_a.N, go_row, go_col);

  // get original local row and col
  int loriginal, lo_row, lo_col;
  global2local(goriginal, &loriginal, mpi.num_procs, desc_a);
  index2coords(loriginal, desc_a.lld, lo_row, lo_col);

  // get process co-ordinates that own the new col
  int orig_prow, orig_pcol;
  procg2l(go_row, go_col, &orig_prow, &orig_pcol, desc_a, mpi);

  // get new global row and col
  int gn_row, gn_col;
  index2coords(gnew, desc_a.N, gn_row, gn_col);

  // get new local row and col
  int lnew, ln_row, ln_col;
  global2local(gnew, &lnew, mpi.num_procs, desc_a);
  index2coords(lnew, desc_a.lld, ln_row, ln_col);
  
  // get the process co-ordinates that own the new row
  int new_prow, new_pcol;
  procg2l(gn_row, gn_col, &new_prow, &new_pcol, desc_a, mpi);

  // number of elements per process
  int num = desc_a.MB / mpi.MP;
  int offset = (int)(block/desc_a.MB);

  // send the original row to the process row that contains the new row
  // communication needs to happen only between rows.
  if (mpi.myrow == orig_prow) {
    Cdgesd2d(mpi.BLACS_CONTEXT, num, 1, &A[lo_row*desc_a.lld + num*offset],
             desc_a.lld, new_prow, mpi.mycol);
  }

  // receive the original row in the process row that contains the new row
  double originalA[num];
  if (mpi.myrow == new_prow) {
    Cdgerv2d(mpi.BLACS_CONTEXT, num, 1, originalA, num, orig_prow, mpi.mycol);
  }

  // send the new row to the process row that contains the original row
  if (mpi.myrow == new_prow) {
    Cdgesd2d(mpi.BLACS_CONTEXT, num, 1, &A[ln_row*desc_a.lld + num*offset],
             num, orig_prow, mpi.mycol);
  }

  // receive the new row in the process row that contains the original row
  double newA[num];
  if (mpi.myrow == orig_prow) {
    Cdgerv2d(mpi.BLACS_CONTEXT, num, 1, newA, num, new_prow, mpi.mycol);
  }

  // copy from new
  if (mpi.myrow == orig_prow) {
    for (int i = 0; i < num; i++) {
      A[lo_row*desc_a.lld + num*offset + i] = newA[i];
    }
  }

  // copy from original
  if (mpi.myrow == new_prow) {
    for (int i = 0; i < num; i++) {
      A[ln_row*desc_a.lld + num*offset + i] = originalA[i];
    }
  }
}

void pivot_column(double *A, int block, int nb, desc desc_a, mpi_desc mpi)
{
  // iterate over columns witin this vertical panel.
  int curr_global;
  double vmax, imax; // imax - max index. vmax - max element.
  double temp_max[2]; // temp storage for max elements for broadcast.
  int imyrow, imycol;

  for (int i = 0; i < nb; ++i) {
    procg2l(block+i, block+i, &imyrow, &imycol, desc_a, mpi);
    if (imycol == mpi.mycol) { // find max element in the columns
      curr_global = (block + i)*desc_a.N + block + i;
      find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, mpi); // idamax
      temp_max[0] = imax; temp_max[1] = vmax;
     
      // broadcast it to all other processes
      Cdgebs2d(mpi.BLACS_CONTEXT, "All", " ", 2, 1, temp_max, 2);
    }
    else {
      // receive broadcast
      Cdgebr2d(mpi.BLACS_CONTEXT, "All", " ", 2, 1, temp_max, 2, mpi.myrow, imycol);
      imax = temp_max[0]; vmax = temp_max[1];
    }
    
    if (imax != curr_global) {
      swap_within_current_panel(A, desc_a, mpi, curr_global, imax, block); // dswap
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
