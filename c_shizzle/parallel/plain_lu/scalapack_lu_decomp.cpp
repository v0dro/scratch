// Implement simple distributed LU decomposition using scalapack.
// This code uses a simple block distribution of data. Not block cyclic.
// author: Sameer Deshmukh (@v0dro)

#include "mpi.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
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
  void pdgetrf_(
                int *m, int *n, double *a, int *ia, int *ja, int *desca,
                int *ipiv,int *info);
}

void print_arr(double *A, int size, string desc, ostream &o)
{
  //o << desc << endl;
  for (int i = 0; i < size; ++i) {
    o << A[i] << " ";
  }
  o << endl;
}

void print_files(double *A, int nrows, int ncols, int myrow, int mycol, string post="")
{
  string n = to_string(myrow*2 + mycol);
  std::ofstream file;

  file.open(n + post + ".txt");
  print_arr(A, nrows*ncols, n, file);
  file.close();
}


int main(int argc, char ** argv)
{
  // MPI init
  MPI_Init(&argc, &argv);
  // end MPI Init

  // BLACS init
  int BLACS_CONTEXT, proc_nrows, proc_ncols, myrow, mycol;
  proc_nrows = 2; proc_ncols = 2;
  Cblacs_get( -1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit( &BLACS_CONTEXT, "Row", proc_nrows, proc_ncols );
  Cblacs_pcoord(BLACS_CONTEXT, proc_id, &myrow, &mycol);
  // end BLACS init

  // matrix properties
  // mat size, blk size, portion of block per process
  int N = 8, nb = 4, process_block_size = 2;
  double* a = (double*)malloc(sizeof(double)*nb*nb);
  // generate matrix data
  int pblock_nrows = nb/2;
  int pblock_ncols = nb/2;
  int num_blocks_per_process = 4;

  // loop over block cols
  for (int bc = 0; bc < pblock_ncols; ++bc) {
    // loop over block rows
    for (int br = 0; br < pblock_nrows; ++br) {
      // loop over number cols
      for (int j = 0; j < pblock_ncols; ++j) {
        // loop over number rows
        for (int i = 0; i < pblock_nrows; ++i) {
          int row_i = myrow*pblock_nrows + i + j * num_blocks_per_process;
          int col_i = bc*num_blocks_per_process + mycol * pblock_ncols + br;
          int index = i + j * pblock_nrows +
            (br + bc * pblock_ncols) * num_blocks_per_process;
          a[index]  = row_i + col_i*N;
        }
      }
    }
  }
  print_files(a, nb, nb, myrow, mycol, "input");
  // end matrix properties

  // create array descriptor
  int desca[9];
  int rsrc = 0, csrc = 0, info;
  descinit_(desca, &N, &N, &nb, &nb, &rsrc, &csrc, &BLACS_CONTEXT, &nb, &info);
  // end create array descriptor

  Cblacs_barrier(BLACS_CONTEXT, "All");

  //int locr = numroc_(&N, &nb, &myrow, &rsrc, &num_procs);
  //std::cout << "locr + mba : " << locr + nb << std::endl;
  // LU decomposition
  int ia = 1, ja = 1;
  int *ipiv = (int*)calloc(sizeof(int), N);
  for (int i = 0; i < N; ++i) {
    ipiv[i] = i+1;
  }
  pdgetrf_(&N, &N, a, &ia, &ja, desca, ipiv, &info);
  // end LU decomposition

  print_files(a, nb, nb, myrow, mycol);
  if (myrow == 0 && mycol == 0) {
    for(int i = 0; i < N; ++i) {
      cout << ipiv[i] << " ";
    }
  }

  if (myrow == 1 && mycol == 1) {
    for(int i = 0; i < N; ++i) {
      cout << ipiv[i] << " ";
    }
  }
  MPI_Finalize();
}
