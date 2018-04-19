// Code for computing an LU decomposition using BLACS
// Author: Sameer Deshmukh (@v0dro)

#include "mpi.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// Compute LU decomposition of the matrix chunk passed in A.
void dist_lu_decomp(
                    int M,
                    int N,
                    double *A,
                    int *ipiv,
                    int nb_M,
                    int nb_N,
                    int CONTEXT)
{
  
}

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
  // end BLACS init

  // matrix properties
  // mat size, blk size, portion of block per process
  int N = 8, nb = 4, process_block_size = 2;
  int num_blocks_per_process = N/process_block_size;
  int block_size_per_process_r = sqrt(num_blocks_per_process);
  int block_size_per_process_c = sqrt(num_blocks_per_process);
  double* a = (double*)malloc(sizeof(double)*nb*nb);
  // generate matrix data
  for (int j = 0; j < nb; ++j) {
    for (int i = 0; i < nb; ++i) {
      int index = i*nb + j;
      int row_i = myrow*nb + i;
      int col_j = mycol*nb + j;
      a[index] = row_i + col_j;
    }
    cout << endl;
  }
  // end matrix properties

  MPI_Finalize();
}
