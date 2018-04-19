// Author: @v0dro
// Desc: Store a distributed matrix in contiguos blocks in an array on each
// process and perform a synchronous LU decomposition.

#include "mpi.h"
#include <cstdlib>
#include <iostream>
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
 
  int numroc_(int*, int*, int*, int*, int*);

  void descinit_(int *desc, const int *m,  const int *n, const int *mb, 
    const int *nb, const int *irsrc, const int *icsrc, const int *ictxt, 
    const int *lld, int *info);

  void pdgemm_( char* TRANSA, char* TRANSB,
                int * M, int * N, int * K,
                double * ALPHA,
                double * A, int * IA, int * JA, int * DESCA,
                double * B, int * IB, int * JB, int * DESCB,
                double * BETA,
                double * C, int * IC, int * JC, int * DESCC );
  
}

void print_block(double *a, int nrows, int ncols, int proc_id)
{
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      cout << "(" << proc_id << "," << i << "," << j << "," << a[i*nrows + j] << ") ";
    }
    cout << endl;
  }
}
// assume square block decomposition
int main(int argc, char ** argv)
{
  // MPI init
  MPI_Init(&argc, &argv);
  // end MPI Init
  
  // BLACS init
  int BLACS_CONTEXT, proc_nrows, proc_ncols, myrow, mycol;
  int proc_id, num_procs;
  proc_nrows = 2; proc_ncols = 2;
  Cblacs_pinfo(&proc_id, &num_procs);
  Cblacs_get( -1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit( &BLACS_CONTEXT, "Row", proc_nrows, proc_ncols );
  Cblacs_pcoord(BLACS_CONTEXT, proc_id, &myrow, &mycol);
  cout << "myrow " << myrow << " mycol " << mycol << endl;
  cout << "procid " << proc_id << " num_procs " << num_procs << endl;
  // end BLACS init

  // matrix properties
  // mat size, blk size, portion of block per process
  int N = 8, nb = 4, process_block_size = 2;
  int num_blocks_per_process = N/process_block_size;
  int block_size_per_process_r = sqrt(num_blocks_per_process);
  int block_size_per_process_c = sqrt(num_blocks_per_process);
  double* a = (double*)malloc(sizeof(double)*nb*nb);

  // generate matrix data
  for (int bcounter_i = 0; bcounter_i < block_size_per_process_r; ++bcounter_i) {
    for (int bcounter_j = 0; bcounter_j < block_size_per_process_c; ++bcounter_j) {
      for (int i = 0; i < process_block_size; ++i) {
        for (int j = 0; j < process_block_size; ++j) {
          int row_i = bcounter_i*num_blocks_per_process +
            myrow*block_size_per_process_r + i;
          int col_j = bcounter_j*num_blocks_per_process +
            mycol*block_size_per_process_c + j;
          int index = (bcounter_i*block_size_per_process_r + bcounter_j)*
            num_blocks_per_process +  i*process_block_size + j;
          // cout << "proc " << proc_id <<  " (" << row_i << "," <<
          //   col_j << "," << index << "," << row_i*N + col_j << ")" <<  endl;
          a[index] = row_i*N + col_j;
        }
      }
    }
  }
  // end matrix properties

  // create array descriptor
  int desca[9];
  int rsrc = 0, csrc = 0, info;
  descinit_(desca, &N, &N, &nb, &nb, &rsrc, &csrc, &BLACS_CONTEXT, &nb, &info);
  // end create array descriptor

  // print matrix blocks
  
  Cblacs_barrier(BLACS_CONTEXT, "All");

  // synchronous LU decomposition
  //   loop over blocks in each process.
  //   
  for (int i = 0; i < nb; ++i) {
    print_block(&a[i*process_block_size*process_block_size],
                 process_block_size, process_block_size, proc_id);
  }
  // end synchronous LU decomposition
  
  MPI_Finalize();
}
