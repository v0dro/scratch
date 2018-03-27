#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

extern "C" {
  /* Cblacs declarations */
  void Cblacs_pinfo(int*, int*);
  void Cblacs_get(int, int, int*);
  void Cblacs_gridinit(int*, const char*, int, int);
  void Cblacs_pcoord(int, int, int*, int*);
  void Cblacs_gridexit(int);
  void Cblacs_barrier(int, const char*);
  void Cdgerv2d(int, int, int, double*, int, int, int);
  void Cdgesd2d(int, int, int, double*, int, int, int);
 
  int numroc_(int*, int*, int*, int*, int*);
}

// compute LU fact using LAPACKE and return result.
void factorize_lu(double *A, int blocksize)
{

}

void p2p_send(double *A, int blocksize, int dest_row, int dest_col)
{
  
}

void p2p_recv(double *recv_block, int blocksize, int src_row, int src_col)
{
}

void matmul(
            double *a,
            double *b,
            double *c,
            int blocksize)
{
  int nrows_a = blocksize, ncols_a = blocksize, nrows_b = blocksize, ncols_b = blocksize;
  cblas_dgemm(
              CblasRowMajor, CblasNoTrans, CblasNoTrans,
              nrows_a, ncols_b, nrows_b,
              1, a, ncols_a, b,
              ncols_b, 1, c, ncols_b);
}


int main(int argc, char ** argv)
{
  int mpi_rank, mpi_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  // .....................................................................................
  // BLACS stuff
  int proc_id, row_rank, col_rank, BLACS_CONTEXT, num_procs;
  int N = sqrt(mpi_size); // NxN process grid
  Cblacs_pinfo(&proc_id, &num_procs);
  Cblacs_get(0,0,&BLACS_CONTEXT);
  Cblacs_gridinit(&BLACS_CONTEXT, "Row-major", N, N);
  Cblacs_pcoord(BLACS_CONTEXT, proc_id, &row_rank, &col_rank); // get grid coords.
  // ....................................................................................

  cout << "row: " << row_rank << "col: " << col_rank << endl;

  // ....................................................................................
  // CONFIGURE MATRIX AND LOAD DATA INTO EACH PROCESS.

  // matrix global configurations
  int NROWS = 4, NCOLS = 4; // total dimensions of matrix.
  int blocksize = 1;
  
  // set initial row/col number of matrix block within this process.
  int row = row_rank, col = col_rank;
  int nblocks = 1; // number of blocks stored in this process.

  // set number of blocks in rows and columns.
  int b_nrows = NROWS/blocksize, b_ncols = NCOLS/blocksize;
  
  // calculate number of blocks that can be stored in this process.
  row += N;
  while(row < b_nrows) {
    nblocks++;
    row += N;
  }
  row = row_rank;

  col += N;
  while (col < b_ncols) {
    nblocks++;
    col += N;
  }
  col = col_rank;

  row += N; col += N;
  while (row < b_nrows && col < b_ncols) {
    nblocks++;
    row += N; col += N;
  }
  row = b_nrows; col = b_ncols;

  // arrays holding row/col numbers of matrix blocks within this process.
  int rows[nblocks], cols[nblocks];
  // data array holding all blocks that belong to this process.
  double *data = (double*)malloc(sizeof(double)*nblocks*blocksize*blocksize);

  // populate rows and cols arrays with absolute row/col numbers of each block.
  // also generate the matrix blocks.
  rows[0] = row_rank;
  cols[0] = col_rank;
  int index = 0;
  while (true) {
    data[index] = 1.0;
    index++;
    if (index < nblocks) {
      rows[index] = rows[index-1] + N;
      cols[index] = cols[index-1] + N;
    }
    else {
      break;
    }
  }

  // ...............................................................................
  
  // ..............................................................................
  // begin actual LU factorization procedure now.
  for (int i = 0; i < nblocks; ++i) { // print data and process row/col rank.
    cout << "process row_rank:" << row_rank <<
      " col_rank: " << col_rank <<
      " blocks: " << nblocks <<
      " data: " << data[i] << endl;
  }

  index = 0;

  while (index < nblocks) {
    if (rows[index] == cols[index]) {  // diagonal block      
      factorize_lu(&data[index], blocksize);// compute LU fact
      // broadcast lower matrix to full row to the right of this block.
      for (col = cols[index]; col < b_ncols; ++col) {
        p2p_send(&data[index], blocksize, rows[index] % N, col % N);
      }
      // broadcast upper matrix to full col below this block.
      for (row = rows[index]; row < b_nrows; ++row) {
        p2p_send(&data[index], blocksize, row % N, cols[index] % N);
      }
      index++;
    }

    // receive a block of data from the diagonal block.
    double *recv_block = (double*)malloc(sizeof(double)*blocksize*blocksize);
    p2p_recv(recv_block, blocksize, rows[index] % N, rows[index] % N);
    // perform multiplication of received block with already present block.
    matmul(recv_block,
           &data[index*blocksize*blocksize],
           &data[index*blocksize*blocksize],
           blocksize);

    // send multiplied block to blocks in the same row.

    // send multiplied block to blocks in the same column.
    // receive a block of data.
    // if the data is coming from a diagonal block, check if the under process
    //    block row and block column in this process are consistent with origin process
    //    such that they are in the same row or column.
    // 
    
      // how to co-ordinate the currently under process block across processes?
      // each process has the absolute row and col numbers of the blocks present in the process.
      // these absolute numbers can be representative of which block has been processed at what time.
      // this can be achieved for keeping a 'processed' counter for each process and incrementing the counter
      //   each time a full row and column is reduced.
      // the counter can be updated each time a row and column is done with reduction. this can be known by a process
      //   listening for a transmission of data.  
  }

  MPI_Finalize();  
}

