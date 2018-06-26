// Author: @v0dro
// Desc: Store a distributed matrix in contiguos blocks in an array on each
// process and perform a synchronous LU decomposition. Assume square block
// decomposition.

// Row major matrix storage in all cases.

#include "sync_lu_decomp.hpp"

void parallel_lu_decomposition()
{
  // BLACS init
  mpi_desc mpi_info;
  initialize_blacs(&mpi_info.BLACS_CONTEXT, &mpi_info.MP, &mpi_info.NP,
                   &mpi_info.myrow, &mpi_info.mycol, &mpi_info.proc_id,
                   &mpi_info.num_procs);
  // end BLACS init
  
  // matrix properties
  // mat size, blk size, portion of block per process
  desc desc_a;
  desc_a.M = 8;
  desc_a.N = 8;
  desc_a.MB = 4;
  desc_a.NB = 4;
  desc_a.BLACS_CONTEXT = mpi_info.BLACS_CONTEXT;
  desc_a.rsrc = 0;
  desc_a.csrc = 0;
  desc_a.lld = numroc_(&desc_a.N, &desc_a.NB, &mpi_info.mycol, &desc_a.rsrc, &mpi_info.num_procs);
  double* a = (double*)malloc(sizeof(double)*desc_a.MB*desc_a.NB);

  // generate matrix data
  generate_data(a, mpi_info.myrow, mpi_info.mycol, desc_a);
  // end matrix properties

  // create array descriptor
  int desca[9]; int info;
  descinit_(desca, &desc_a.M, &desc_a.N, &desc_a.MB, &desc_a.NB,
            &desc_a.rsrc, &desc_a.csrc, &mpi_info.BLACS_CONTEXT, &desc_a.lld, &info);
  // end create array descriptor
 
  // synchronous LU decomposition
  //   loop over blocks in each process.
  // This loop iterates over each block in each process.
  int *ipiv;
  ipiv = (int*)malloc(sizeof(int)*desc_a.N);

  for (int i = 0; i < desc_a.N; ++i) {
    ipiv[i] = i;
  }

  print_files(a, desc_a.MB, desc_a.NB, mpi_info.myrow, mpi_info.mycol, "input");
  int ia ;
  diagonal_block_lu(a, ipiv, desca, desc_a, mpi_info);
 
  print_files(a, desc_a.MB, desc_a.NB, mpi_info.myrow, mpi_info.mycol);  
  // end synchronous LU decomposition  
}

void run_unit_tests()
{
  test_global2local();
  test_local2global();
}

int main(int argc, char ** argv)
{
  // MPI init
  MPI_Init(&argc, &argv);
  // end MPI Init

  err_file.open("err_file.txt");
  
  if (TESTING) {
    run_unit_tests();
  }
  else {
    parallel_lu_decomposition();
  }

  err_file.close();
  MPI_Finalize();
}
