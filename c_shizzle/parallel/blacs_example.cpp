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

  // because of the way in which rows and cols are arranged in each process, it
  // is possible to iterate over the rows and cols arrays and not block
  // unnecessarily.
  
  while (index < nblocks) {
    // factor some blocking condition that will make sure that this block
    // is not factorized unless the block above and to its left transmit data to it.
    int block_row = rows[index];
    int block_col = cols[index];
    if (block_row == block_col) {  // diagonal block
      // don't wait for transmission if this is the (0,0) block.
      if (block_row != 0 && block_col != 0) {
        double *recv_data_diag_top = (double*)malloc(sizeof(double)*blocksize*blocksize);
        double *recv_data_diag_left = (double*)malloc(sizeof(double)*blocksize*blocksize);
        // get data from block above.
        p2p_recv(recv_data_diag_top, blocksize, (block_row - 1) % N, block_col % N);
        // get data from block to the left.
        p2p_recv(recv_data_diag_left, blocksize, block_row % N, (block_col - 1) % N);
        // multiply data from left and top and subtract from this block i.e. (block_row, block_col)
        matmul();
      }

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

    if (index > nblocks) break;

    // receive a block of data from the diagonal block, multiply and send to lower right block.
    block_row = rows[index]; block_col = cols[index];
    recv_block = (double*)malloc(sizeof(double)*blocksize*blocksize);
    if (block_row > block_col) { // block below the diagonal
      p2p_recv(recv_block, blocksize, block_col % N, block_col % N);
    }
    else if (block_row < block_col) { // block to the right of diagonal
      p2p_recv(recv_block, blocksize, block_row % N, block_row % N);
    }

    // perform multiplication of received block with already present block.
    matmul(recv_block,
           &data[index*blocksize*blocksize],
           &data[index*blocksize*blocksize],
           blocksize);

    if (block_row > block_col) { // block in lower part of matrix so communicate to all rows
      for (col = block_col; col < b_ncols; ++col) {
        p2p_send();
      }
    }
    else if (block_row < block_col) { // block in upper part so communicate to all cols.
      for (row = block_row; row < b_nrows; ++row) {
        p2p_send();
      }
    }
    
    index++;

    if (index > nblocks) break;
    
    block_row = rows[index]; block_col = cols[index]; 
    // receive blocks from row and col, multiply them and subtract product from this block.
    // how to know which block is sending the data?
    p2p_recv(recv_data_diag_top, );

  }

  // it should not so happen that we just keep iterating over the blocks in a process and assume
  //   that they happen in a particular order. In order to ensure this there should be guards
  //   placed around every specific block to check if the block belongs to a particular place
  //   in the matrix. Only then should the relevant code for that particular block be called.

  // ANY block except diagonal block HAS to receive data from a diagonal and only _then_ can it
  //   transmit data to the lower left blocks. Therefore there is no need to check whether the
  //   block is present in the row/col of the diagonal element or not, it will happen sequentially
  //   anyway.

  // for blocks that are in the lower right corner of the matrix, the index should not be
  //   incremented. the data transmission should just be received, multiplied and subtracted
  //   from this block. The exception to this rule is the diagonal block that should fire
  //   its data as soon as it receives data from blocks in the row and cols above it.

  // however, now a problem is how we will get to know whether the transmission being received
  //   into the diagonal block? Because, the block must fire its data only after receiving data
  //   from blocks that are immediately above and to its left. The case where it receives data
  //   from blocks that are further than the immediate row and column should be treated like
  //   the regular reduction operation and should not lead to firing of blocks.

  MPI_Finalize();  
}

