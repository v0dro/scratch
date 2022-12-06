#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include "mpi.h"
#include "cblas.h"

extern "C" {
  /* Cblacs declarations */
  void Cblacs_pinfo(int*, int*);
  void Cblacs_get(int, int, int*);
  void Cblacs_gridinit(int*, const char*, int, int);
  void Cblacs_pcoord(int, int, int*, int*);
  void Cblacs_gridexit(int);
  void Cblacs_barrier(int, const char*);

  int numroc_(int*, int*, int*, int*, int*);

  // init descriptor for scalapack matrices.
  void descinit_(int *desc, const int *m,  const int *n, const int *mb,
                 const int *nb, const int *irsrc, const int *icsrc, const int *BLACS_CONTEXT,
                 const int *lld, int *info);
  // Pivoted QR. A is replaced with elementary reflectors of A.
  void	pdgeqpf_(const int* m, const int* n, double* a, const int* ia,
                 const int* ja, const int* desca, int* ipiv, double* tau,
                 double* work, const int* lwork, int* info);

  // Obtain QR factors from elementary reflectors obtained from geqpf.
  void	pdorgqr_(const int* m, const int* n, const int* k,
                 double* a, const int* ia, const int* ja,
                 const int* desca, const double* tau, double* work,
                 const int* lwork, int* info);
}

int
indxg2l(int INDXGLOB, int NB, int NPROCS) {
  return NB * ((INDXGLOB - 1) / ( NB * NPROCS)) + (INDXGLOB - 1) % NB + 1;
}

int
indxl2g(int indxloc, int nb, int iproc, int isrcproc, int nprocs) {
  return nprocs * nb * ((indxloc - 1) / nb) +
    (indxloc-1) % nb + ((nprocs + iproc - isrcproc) % nprocs) * nb + 1;
}

int
indxg2p(int INDXGLOB, int NB, int ISRCPROC, int NPROCS) {
  return (ISRCPROC + (INDXGLOB - 1) / NB) % NPROCS;
}


int MYROW, MYCOL, MPISIZE, MPIRANK;
int MPIGRID[2];
int ONE=1, ZERO = 0, MINUS_ONE = -1;
int info;

int RAND[9], DENSE[9], AY[9];
int N, P;

double *DENSE_MEM, *RAND_MEM, *AY_MEM;

// generate three matrices DENSE, AY, and RAND. Perform block wise multiplication
// to generate AY slice-by-slice.

int main(int argc, char** argv) {
  N = atoi(argv[1]);
  P = atoi(argv[2]);

  // MPI init
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Dims_create(MPISIZE, 2, MPIGRID);

  int BLACS_CONTEXT;

  Cblacs_get(-1, 0, &BLACS_CONTEXT);
  Cblacs_gridinit(&BLACS_CONTEXT, "Row", MPIGRID[0], MPIGRID[1]);
  Cblacs_pcoord(BLACS_CONTEXT, MPIRANK, &MYROW, &MYCOL);

  int DENSE_NB_ROW = N / MPIGRID[0], DENSE_NB_COL = P / MPIGRID[1];
  int RAND_NB_ROW = N / MPIGRID[0], RAND_NB_COL = P / MPIGRID[1];
  int AY_NB_ROW = N / MPIGRID[0], AY_NB_COL = P / MPIGRID[1];

  int DENSE_local_rows = numroc_(&N, &DENSE_NB_ROW, &MYROW, &ZERO, &MPIGRID[0]);
  int DENSE_local_cols = numroc_(&N, &DENSE_NBCOL, &MYCOL, &ZERO, &MPIGRID[1]);
  DENSE_MEM = new double[DENSE_local_rows * DENSE_local_cols];


  descinit_(DENSE, &N, &P, &DENSE_NB_ROW, &DENSE_NB_COL, &ZERO, &ZERO,
            &BLACS_CONTEXT, &DENSE_local_rows, &info);

  MPI_Finalize();
}
