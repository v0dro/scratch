// Author: @v0dro
// Desc: Store a distributed matrix in contiguos blocks in an array on each
// process and perform a synchronous LU decomposition. Assume square block
// decomposition

#include "sync_lu_decomp.hpp"

int main(int argc, char ** argv)
{
  // MPI init
  MPI_Init(&argc, &argv);
  // end MPI Init
  
  // BLACS init
  int BLACS_CONTEXT, proc_nrows, proc_ncols, myrow, mycol;
  int proc_id, num_procs;
  initialize_blacs(&BLACS_CONTEXT, &proc_nrows, &proc_ncols, &myrow, &mycol,
                   &proc_id, &num_procs);
  // end BLACS init
  
  // matrix properties
  // mat size, blk size, portion of block per process
  int N = 8, nb = 4, process_block_size = 2;
  int num_blocks_per_process = N/process_block_size;
  int block_size_per_process_r = sqrt(num_blocks_per_process);
  int block_size_per_process_c = sqrt(num_blocks_per_process);
  double* a = (double*)malloc(sizeof(double)*nb*nb);

  // generate matrix data
  generate_data(a, block_size_per_process_r, block_size_per_process_c,
                process_block_size, num_blocks_per_process, myrow, mycol, N);
  // end matrix properties

  // create array descriptor
  int desca[9];
  int rsrc = 0, csrc = 0, info;
  int b_fac = 2;
  int lld = numroc_(&N, &b_fac, &myrow, &rsrc, &num_procs);
  lld = 4;
  descinit_(desca, &N, &N, &b_fac, &b_fac, &rsrc, &csrc, &BLACS_CONTEXT, &lld, &info);
  // end create array descriptor
 
  // synchronous LU decomposition
  //   loop over blocks in each process.
  // This loop iterates over each block in each process.
  int *ipiv;
  ipiv = (int*)malloc(sizeof(int)*N);

  // loop over matrix blocks.
  for (int ia = 0; ia < nb; ia += nb) {
    diagonal_block_lu(a, ia, nb, N, ipiv, &BLACS_CONTEXT, desca);
    // compute LU of diagonal block.
    // broadcast block along rows and cols.
    // broadcast row and col blocks along the lower right block of the matrix and multiply.
    // 
  }

  print_files(a, nb, nb, myrow, mycol);

  
  // end synchronous LU decomposition

  
  
  MPI_Finalize();
}
