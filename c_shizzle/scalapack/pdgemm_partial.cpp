#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include "mpi.h"
#include <mkl.h>

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


  void pdgemm_(const char *TRANSA, const char *TRANSB,
               const int *M, const int *N, const int *K,
               const double *ALPHA,
               double *A, int *IA, int *JA, int *DESCA,
               double *B, int *IB, int *JB, int *DESCB,
               double *	BETA,
               double *	C, int *IC, int *JC, int *DESCC);
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

const char NOTRANS = 'N';

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

  int DENSE_NB_ROW = 100, DENSE_NB_COL = P / MPIGRID[1];
  int DENSE_local_rows = numroc_(&N, &DENSE_NB_ROW, &MYROW, &ZERO, &MPIGRID[0]);
  int DENSE_local_cols = numroc_(&N, &DENSE_NB_COL, &MYCOL, &ZERO, &MPIGRID[1]);
  DENSE_MEM = new double[DENSE_local_rows * DENSE_local_cols];
  descinit_(DENSE, &N, &N, &DENSE_NB_ROW, &DENSE_NB_COL, &ZERO, &ZERO,
            &BLACS_CONTEXT, &DENSE_local_rows, &info);

  int RAND_NB_ROW = 100, RAND_NB_COL = P / MPIGRID[1];
  int RAND_local_rows = numroc_(&N, &RAND_NB_ROW, &MYROW, &ZERO, &MPIGRID[0]);
  int RAND_local_cols = numroc_(&P, &RAND_NB_COL, &MYCOL, &ZERO, &MPIGRID[1]);
  RAND_MEM = new double[RAND_local_rows * RAND_local_cols];
  descinit_(RAND, &N, &P, &RAND_NB_ROW, &RAND_NB_COL, &ZERO, &ZERO,
            &BLACS_CONTEXT, &RAND_local_rows, &info);


  int AY_NB_ROW = 100, AY_NB_COL = P / MPIGRID[1];
  int AY_local_rows = numroc_(&N, &AY_NB_ROW, &MYROW, &ZERO, &MPIGRID[0]);
  int AY_local_cols = numroc_(&P, &AY_NB_COL, &MYCOL, &ZERO, &MPIGRID[1]);
  AY_MEM = new double[AY_local_rows * AY_local_cols];
  descinit_(AY, &N, &P, &AY_NB_ROW, &AY_NB_COL, &ZERO, &ZERO,
            &BLACS_CONTEXT, &AY_local_rows, &info);

  // seed with the rank to prevent the same sequence on every proc.
  std::mt19937 gen(MPIRANK);
  std::uniform_real_distribution<double> dist(0.0, 1.0);

#pragma omp parallel for
  for (int i = 0; i < RAND_local_rows; ++i) {
#pragma omp parallel for
    for (int j = 0; j < RAND_local_cols; ++j) {
      RAND_MEM[i + j * RAND_local_rows] = dist(gen);
    }
  }

  double ALPHA = 1.0;
  double BETA = 1.0;
  MKL_INT block_size = 100;
  int nblocks = N / block_size;

  for (int block = 0; block < nblocks; ++block) {
    for (int j = 1; j < nblocks; ++j) {

      MKL_INT IA = block_size * block + 1;
      MKL_INT JA = block_size * j + 1;
      // MKL_INT JA = 1;

      MKL_INT IB = block_size * j + 1;
      MKL_INT JB = 1;

      MKL_INT IC = block_size * block + 1;
      MKL_INT JC = 1;

      pdgemm_(&NOTRANS, &NOTRANS, &block_size, &P, &block_size,
              &ALPHA,
              DENSE_MEM, &IA, &JA, DENSE,
              RAND_MEM, &IB, &JB, RAND,
              &BETA,
              AY_MEM, &IC, &JC, AY);
    }
  }

  delete[] RAND_MEM;
  delete[] DENSE_MEM;
  delete[] AY_MEM;

  MPI_Finalize();
}
