#include "implementation.hpp"

static int c = 0;

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
  int imyrow, imycol;
  double send[2], receive[2];

  if (mpi.proc_id == 0) {
    cout << "proc: " << mpi.proc_id << " diag : " << block+i << endl;
  }
  
  procg2l(block+i, block+i, &imyrow, &imycol, desc_a, mpi);

  if (imycol == mpi.mycol) { // find the max in the same column
    find_local_max_element(A, block, i, desc_a, lvmax, limax, mpi.num_procs);
    // broadcast local max indexes and numbers along the process columns.
    double max[mpi.MP*2];
    int temp_imax;
    local2global(limax, &temp_imax, mpi.myrow, mpi.mycol, mpi.num_procs, desc_a);
    max[mpi.myrow*mpi.MP]     = lvmax;
    max[mpi.myrow*mpi.MP + 1] = temp_imax;
    // Send the local imax and vmax to all processes in the same column:
    for (int r = 0; r < mpi.MP; r++) {
      if (r != mpi.myrow)
        Cdgesd2d(mpi.BLACS_CONTEXT, 2, 1, &max[mpi.myrow*mpi.MP], 2, r, mpi.mycol);
    }
    // Receive the imax and vmax of all processes in the same column:
    for (int r = 0; r < mpi.MP; r++) {
      if (r != mpi.myrow)
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

    send[0] = *vmax;
    send[1] = *imax;
    // send to each process in the same row (except this process)
    for (int i = 0; i < mpi.NP; ++i) {
      if (i != imycol) {
        Cdgesd2d(mpi.BLACS_CONTEXT, 2, 1, send, 2, mpi.myrow, i);
      }
    }
  }
  else { // other process listen for the max elements
    Cdgerv2d(mpi.BLACS_CONTEXT, 2, 1, receive, 2, mpi.myrow, imycol);
    *vmax = receive[0]; *imax = receive[1];
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
             1, new_prow, mpi.mycol);
  }

  // receive the original row in the process row that contains the new row
  double originalA[num];
  if (mpi.myrow == new_prow) {
    Cdgerv2d(mpi.BLACS_CONTEXT, num, 1, originalA, 1, orig_prow, mpi.mycol);
  }

  // send the new row to the process row that contains the original row
  if (mpi.myrow == new_prow) {
    Cdgesd2d(mpi.BLACS_CONTEXT, num, 1, &A[ln_row*desc_a.lld + num*offset],
             1, orig_prow, mpi.mycol);
  }

  // receive the new row in the process row that contains the original row
  double newA[num];
  if (mpi.myrow == orig_prow) {
    Cdgerv2d(mpi.BLACS_CONTEXT, num, 1, newA, 1, new_prow, mpi.mycol);
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

  if (mpi.myrow == 1 && mpi.mycol == 0)
    cout << "diag :: " << diag << endl;
  // iterate over the row and send numbers along columns
  gstart = diag - diag % max_panel_size;
  for (int col = gstart; col < block + nb; col += max_panel_size) {
    procg2l(diag, col, &newrow, &newcol, desc_a, mpi);
    
    if (mpi.mycol == newcol) { // send data along the column
      g2l(diag, col, lrow, lcol, desc_a, mpi);
      panel_start = lrow*desc_a.lld + lcol;
      
      if (mpi.myrow == newrow) { // the diagonal has to rest on this particular process
        //cout << "myrow " << mpi.myrow << " mycol " << mpi.mycol <<
        //" A " << A[panel_start] << " " << A[panel_start + 1] << endl;
          //cout << "panel start : " << panel_start << endl;
        for (int proc_row = 0; proc_row < mpi.MP; proc_row++) {
          Cdgesd2d(mpi.BLACS_CONTEXT, max_panel_size, 1, &A[panel_start], desc_a.lld,
                   proc_row, newcol);
        }
      }
      Cdgerv2d(
               mpi.BLACS_CONTEXT, max_panel_size, 1,
               &temp_row[row_counter*max_panel_size],
               desc_a.lld, newrow, newcol);
      row_counter++;
    }
  }

  // iterate over the col and send numbers along the rows
  for (int row = gstart; row < desc_a.M; row += max_panel_size) {
    procg2l(row, diag, &newrow, &newcol, desc_a, mpi);
    double temp[max_panel_size];

    if (mpi.myrow == newrow) {
      g2l(row, diag, lrow, lcol, desc_a, mpi);
      panel_start = lrow*desc_a.lld + lcol;

      if (mpi.mycol == newcol) {
        for (int i = 0; i < max_panel_size; i++) {
          temp[i] = A[panel_start + i*(desc_a.lld)];
        }
        
        for (int proc_col = 0; proc_col < mpi.NP; proc_col++) {
          Cdgesd2d(mpi.BLACS_CONTEXT, max_panel_size, 1, temp, desc_a.lld, mpi.myrow, proc_col);
        }
      }
      Cdgerv2d(mpi.BLACS_CONTEXT, max_panel_size, 1,
               &temp_col[col_counter*max_panel_size],
               desc_a.lld, newrow, newcol);
      col_counter++;
    }
  }

  if (mpi.myrow == 1 && mpi.mycol == 1) {
    //cout << "printing temp_row: ";
    for (int i = 0; i < max_panel_size*2; i++) {
      // cout << temp_row[i] << " ";
    }

    //  cout << endl << "printing temp_col: ";
    for (int i = 0; i < max_panel_size*2; i++) {
      //  cout << temp_col[i] << " ";
    }
    cout << endl;
  }

  // reduce each element with values in temp_row and temp_col. Even though there might
  //   multiple kinds of blocks in a single matrix, we can reduce them all since the
  //   diagonal row/col always lies in top left part.
  
  // most general case where all elements in the process block should be reduced
  int ullrow, ullcol; // upper left local row and col
  int ulgrow, ulgcol;
  int lrlrow, lrlcol; // lower right local row and col
  int lrgrow, lrgcol;
  int mb_row, mb_col; // matrix block inside which the diagonal element is present

  // get local row and col of upper left corner of proc block based matrix block of diag row/col
  mat_block(diag, diag, mb_row, mb_col, desc_a);
  // get global (row, col) of upper left corner of process block
  ullrow = mb_row * max_panel_size;
  ullcol = mb_col * max_panel_size;
  l2g(ullrow, ullcol, ulgrow, ulgcol, desc_a, mpi);

  // get global (row, col) of lower right corner of process block
  lrlrow = ullrow + max_panel_size;
  lrlcol = ullcol + max_panel_size;
  l2g(lrlrow, lrlcol, lrgrow, lrgcol, desc_a, mpi);

  // counters for temp arrays.
  int tr=0, tc=0;
  // detect that block is of nature where it contains no diag row/col.
  if (ulgrow > diag && ulgcol > diag && lrgrow > diag && lrgcol > diag) {
    // iterate over all block-cyclic matrix blocks that satisfy this condition
    for (int i = ullrow; i < desc_a.lld; i++) {
      for (int j = ullcol; j < ullcol + max_panel_size; j++) { // FIXME : upper bound
        A[i*desc_a.lld + j] -= temp_row[tr] * temp_col[tc];
        tr++;
      }
      tr = 0;
      tc++;
    }
  }
  // case where intersection of the diagonal row and column is in the proc block
  else if (ulgrow <= diag && ulgcol <= diag && lrgrow > diag && lrgcol > diag) {
    g2l(diag+1, diag+1, lrow, lcol, desc_a, mpi);
    tr = (diag+1) % max_panel_size; tc = (diag+1) % max_panel_size;
    
    for (int i = lrow; i < desc_a.lld; i++) {
      for (int j = lcol; j < max_panel_size; j++) { // FIXME : upper bound
        A[i*desc_a.lld + j] -= temp_row[tr] * temp_col[tc];
        tr++;
      }
      tr = (diag+1) % max_panel_size;
      tc++;
    }
  }
  // case where only the diagonal col is in the proc block
  else if (ulgrow > diag && ulgcol <= diag && lrgrow > diag && lrgcol > diag) {
    g2l(ulgrow, diag+1, lrow, lcol, desc_a, mpi);
    tr = (diag+1) % max_panel_size; tc = 0;

    for (int i = lrow; i < desc_a.lld; i++) {
      for (int j = lcol; j < max_panel_size; j++) { // FIXME : upper bound
        A[i*desc_a.lld + j] -= temp_row[tr] * temp_col[tc];
        tr++;
      }
      tr = (diag+1) % max_panel_size;
      tc++;
    }
  }
  // case where only the diagonal row is in the proc block
  else if (ulgrow >= diag && ulgcol >= diag && lrgrow > diag && lrgcol > diag) {
    g2l(diag+1, ulgcol, lrow, lcol, desc_a, mpi);
    tr = 0; tc = (diag+1) % max_panel_size;

    for (int i = lrow; i < desc_a.lld; i++) {
      for (int j = lcol; j < max_panel_size; j++) {
        A[i*desc_a.lld + j] -= temp_row[tr] * temp_col[tc];
        tr++;
      }
      tr = 0;
      tc++;
    }
  }

}

void pivot_column(double *A, int block, int nb, int * ipiv,  desc desc_a, mpi_desc mpi)
{
  // iterate over columns witin this vertical panel.
  int curr_global;
  double vmax, imax; // imax - max index. vmax - max element.
  double temp_max[2]; // temp storage for max elements for broadcast.
  int imyrow, imycol;

  for (int i = 0; i < nb; ++i) {
    // get process row and col of diagonal element at (block+i,block+i)

    // compute global array block of diagonal element.
    curr_global = (block + i)*desc_a.N + block + i;
    // vmax = 1; imax=1;
    find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, mpi); // idamax
    // if (imycol == mpi.mycol) { // find max element in the columns
    //   find_max_element_in_col(A, block, i, &imax, &vmax, desc_a, mpi); // idamax
    //   temp_max[0] = imax; temp_max[1] = vmax;
     
    //   // broadcast it to all other processes
    //   //cout << "pre broadcast : " << temp_max[0] << " " << temp_max[1] << endl;
    //   Cdgebs2d(mpi.BLACS_CONTEXT, "All", " ", 2, 1, temp_max, 2);
    // }
    // else {
    //   // receive broadcast
    //   Cdgebr2d(mpi.BLACS_CONTEXT, "All", " ", 2, 1, temp_max, 2, mpi.myrow, imycol);
    //   //cout << "post broadcast : " << temp_max[0] << " " << temp_max[1] << endl;
    //   imax = temp_max[0]; vmax = temp_max[1];
    // }
    update_ipiv(ipiv, block+i, imax, desc_a);
    
    if (imax != curr_global) {
      //cout << "block " << block << " curr_global " << curr_global << endl;
      swap_within_current_panel(A, desc_a, mpi, curr_global, imax, block); // dswap
    }
    scale_by_pivot(A, block+i, vmax, desc_a, mpi); // dscal
    print_files(A, desc_a.MB, desc_a.NB, mpi.myrow, mpi.mycol, "panel_update" + to_string(c));
    c += 1;
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
void diagonal_block_lu(double *A, int *ipiv, int *desca, desc desc_a, mpi_desc mpi)
{
  if (COL_MAJOR) {
    int info;
    int ia = 1;
    pdgetrf_(&desc_a.M, &desc_a.N, A, &ia, &ia, desca, ipiv, &info);
  }
  else if (ROW_MAJOR) {
    for (int block = 0; block < desc_a.N ; block += desc_a.NB) {
      pivot_column(A, block, desc_a.NB, ipiv, desc_a, mpi);
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
