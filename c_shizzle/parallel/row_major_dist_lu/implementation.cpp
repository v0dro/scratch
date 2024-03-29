#include "implementation.hpp"

// block - global block number
// i - global column number within panel
// lvmax [out] - local max element
// limax [out] - local max index
void find_local_max_element(double *A, int block, int i, desc desc_a,
                            double &lvmax, double &limax, mpi_desc mpi)
{ 
  int lrow, lcol;
  g2l(block+i, block+i, lrow, lcol, desc_a, mpi);

  int temp = block + i;
  
  // get the co-ordinate in this process closest to the diag (below)
  while (lrow == -1) {
    temp += 1;
    g2l(temp, block+i, lrow, lcol, desc_a, mpi);
  }

  // return if this is the lower right-most process
  if (temp >= desc_a.N) {
    lvmax = 0; limax = 0; return;
  }

  int global_index = (temp)*desc_a.N + (block+i);
  int local_index;
  global2local(global_index, &local_index, mpi.num_procs, desc_a);
    
  lvmax = A[local_index];
  limax = local_index;

  for (int row = lrow; row < desc_a.lld; row += 1) {
    if (abs(lvmax) < abs(A[local_index])) {
      lvmax = A[local_index];
      limax = local_index;
    }
    local_index += desc_a.lld;
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
  int imyrow, imycol;
  double _send[2], receive[2];
  MPI_Status status;
  MPI_Request req[mpi.MP];
  
  procg2l(block+i, block+i, &imyrow, &imycol, desc_a, mpi);

  if (imycol == mpi.mycol) { // find the max in the same column
    find_local_max_element(A, block, i, desc_a, lvmax, limax, mpi);
    // broadcast local max indexes and numbers along the process columns.
    double max[mpi.MP*2];
    double final[2];
    int temp_imax;
    local2global(limax, &temp_imax, mpi.myrow, mpi.mycol, mpi.num_procs, desc_a);

    final[0] = lvmax;
    final[1] = temp_imax;

    // Send the local imax and vmax to all processes in the same column:
    for (int r = 0; r < mpi.MP; r++) {
      isend(final, r, mpi.mycol, 2, TAG_0, MPI_DOUBLE, mpi, &req[r]);
    }
    
    // Receive the imax and vmax
    for (int r = 0; r < mpi.MP; r++) {
      recv(&max[r*mpi.MP], r, mpi.mycol, 2, TAG_0, MPI_DOUBLE, mpi, &status);
    }

    for (int i = 0; i < mpi.MP; i++)
      MPI_Wait(&req[i], &status);

    // choose max element and corresponding global index among broadcasted numbers.
    *vmax = max[0]; *imax = max[1];
    for (int i = 2; i < mpi.MP*2; i += 2) {
      if (abs(*vmax) < abs(max[i])) {
        *vmax = max[i];
        *imax = max[i+1];
      }
    }

    _send[0] = *vmax;
    _send[1] = *imax;
    // send to each process in the same row (except this process)
    for (int i = 0; i < mpi.NP; ++i) {
      if (i != imycol) {
        send(_send, mpi.myrow, i, 2, 1, MPI_DOUBLE, mpi);
      }
    }
  }
  else { // other process listen for the max elements
    recv(receive, mpi.myrow, imycol, 2, 1, MPI_DOUBLE, mpi, &status);
    *vmax = receive[0]; *imax = receive[1];
  }
}

// Swap the rows within this current panel.
void swap_within_current_panel(double *A, desc desc_a, mpi_desc mpi,
                               int goriginal, int gnew, int block)
{
  MPI_Status status;
  MPI_Request req;
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
    isend(&A[lo_row*desc_a.lld + num*offset], new_prow, mpi.mycol, num, TAG_2,
          MPI_DOUBLE, mpi, &req);
  }

  // receive the original row in the process row that contains the new row
  double originalA[num];
  if (mpi.myrow == new_prow) {
    recv(originalA, orig_prow, mpi.mycol, num, TAG_2, MPI_DOUBLE, mpi, &status);
  }

  if (mpi.myrow == orig_prow)
    MPI_Wait(&req, &status);

  // send the new row to the process row that contains the original row
  if (mpi.myrow == new_prow) {
    isend(&A[ln_row*desc_a.lld + num*offset], orig_prow, mpi.mycol, num,
          TAG_1, MPI_DOUBLE, mpi, &req);
  }

  // receive the new row in the process row that contains the original row
  double newA[num];
  if (mpi.myrow == orig_prow) {
    recv(newA, new_prow, mpi.mycol, num, TAG_1, MPI_DOUBLE, mpi, &status);
  }

  if (mpi.myrow == new_prow)
    MPI_Wait(&req, &status);

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

void update_ipiv(int* ipiv, int orig_index, double imax, desc desc_a)
{
  int grow, gcol;
  index2coords(imax, desc_a.N, grow, gcol);
  ipiv[orig_index] = grow;
}

// grow - current global row
// vmax - current max element in the column
void scale_by_pivot(double *A, int diag, double vmax, desc desc_a, mpi_desc mpi)
{
  int lrow, lcol;
  
  for (int i = diag+1; i < desc_a.N; i++) {
    g2l(i, diag, lrow, lcol, desc_a, mpi);
    if (lrow != -1 && lcol != -1) {
      A[lrow*desc_a.lld + lcol] /= vmax;
    }
  }
}

// update the remaining part of the panel matrix within the panel of width NB.
void update_panel_submatrix(double *A, int diag, int block, int nb, desc desc_a, mpi_desc mpi)
{
  int max_panel_size = desc_a.MB / mpi.MP;
  // store elements that are received from the diagonal row
  double *temp_row = (double*)malloc(sizeof(double)*max_panel_size*mpi.MP);
  // store elements that are received from the diagonal column
  double *temp_col = (double*)malloc(sizeof(double)*max_panel_size*mpi.NP);
  int newrow, newcol;
  int grow, gcol, lrow, lcol;
  int panel_start, gstart, row_counter=0, col_counter=0;
  MPI_Status status;
  MPI_Request req[mpi.MP];

  // iterate over the row and send numbers along columns
  procg2l(diag, mpi.mycol*max_panel_size, &newrow, &newcol, desc_a, mpi);
  
  if (mpi.mycol == newcol) { // send data along the column
    g2l(diag, mpi.mycol*max_panel_size, lrow, lcol, desc_a, mpi);
    panel_start = lrow*desc_a.lld + lcol;
      
    if (mpi.myrow == newrow) { // the diagonal row has to rest on this particular process
      for (int proc_row = 0; proc_row < mpi.MP; proc_row++) {
        isend(&A[panel_start], proc_row, newcol, max_panel_size*mpi.MP, TAG_0,
              MPI_DOUBLE, mpi, &req[proc_row]);
      }
    }
    
    recv(temp_row, newrow, newcol, max_panel_size*mpi.MP,
         TAG_0, MPI_DOUBLE, mpi, &status);
  }

  // iterate over the col and send numbers along the rows

  procg2l(mpi.myrow*max_panel_size, diag, &newrow, &newcol, desc_a, mpi);
  double temp[max_panel_size*mpi.MP];
  
  if (mpi.myrow == newrow) {
    g2l(mpi.myrow*max_panel_size, diag, lrow, lcol, desc_a, mpi);
    panel_start = lrow*desc_a.lld + lcol;

    if (mpi.mycol == newcol) {
      for (int i = 0; i < max_panel_size*mpi.MP; i++) {
        temp[i] = A[panel_start + i*(desc_a.lld)];
      }
        
      for (int proc_col = 0; proc_col < mpi.NP; proc_col++) {
        isend(temp, mpi.myrow, proc_col, max_panel_size*mpi.MP, TAG_1, MPI_DOUBLE,
              mpi, &req[proc_col]);
      }
    }
    
    recv(temp_col, newrow, newcol, max_panel_size*mpi.MP,
         TAG_1, MPI_DOUBLE, mpi, &status);
  }

  // reduce each element with values in temp_row and temp_col. Even though there might
  //   multiple kinds of blocks in a single matrix, we can reduce them all since the
  //   diagonal row/col always lies in top left part.
  for (int row = diag+1; row < desc_a.M; row++) {
    for (int col = diag+1; col < block+nb; col++) {
      g2l(row, col, lrow, lcol, desc_a, mpi);

      if (lrow != -1) { // if this element is present in this process
        A[lrow*desc_a.lld + lcol] -= temp_row[lcol] * temp_col[lrow];
      }
    }
  }
}

void pivot_column(double *A, int block, int nb, int * ipiv,  desc desc_a, mpi_desc mpi)
{
  // iterate over columns witin this vertical panel.
  int curr_global;
  double vmax, imax; // imax - max index. vmax - max element.

  for (int i = 0; i < nb; ++i) {
    // compute global array block of diagonal element.
    curr_global = (block + i)*desc_a.N + block + i;
    
    find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, mpi); // idamax
    update_ipiv(ipiv, block+i, imax, desc_a);
    
    if (imax != curr_global) {
      swap_within_current_panel(A, desc_a, mpi, curr_global, imax, block); // dswap
    }
    scale_by_pivot(A, block+i, vmax, desc_a, mpi); // dscal
    update_panel_submatrix(A, block+i, block, nb, desc_a, mpi); // dger
  }
}

// Calculate the LU decomposition.
//
// Each block of the matrix is subdivided among all processes in a block cyclic manner.
//   The data distribution format is the same as that used in scalapack. The 'unit of computation'
//   is the matrix block of size MB * NB, which belongs to a global matrix of size M * N.
//
// The implementation is a right-looking block LU decomposition.
void diagonal_block_lu(double *A, int *ipiv, desc desc_a, mpi_desc mpi)
{
  for (int block = 0; block < desc_a.N ; block += desc_a.NB) {
    pivot_column(A, block, desc_a.NB, ipiv, desc_a, mpi);
    // swap_panels();
    // update_upper_panel();
    // update_trailing_submatrix();
  }
}
