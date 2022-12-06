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

int main(int argc, char** argv) {
  // MPI init
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Dims_create(MPISIZE, 2, MPIGRID);
  // end MPI Init

  // BLACS init
  int BLACS_CONTEXT;

  Cblacs_get(-1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit(&BLACS_CONTEXT, "Row", MPIGRID[0], MPIGRID[1]);
  Cblacs_pcoord(BLACS_CONTEXT, MPIRANK, &MYROW, &MYCOL);

  int N = 40, NB = 20;
  int A_lrows = numroc_(&N, &NB, &MYROW, &ZERO, &MPIGRID[0]);
  int A_lcols = numroc_(&N, &NB, &MYCOL, &ZERO, &MPIGRID[1]);

  int VECTOR_lrows = numroc_(&N, &NB, &MYROW, &ZERO, &MPIGRID[0]);
  int VECTOR_lcols = numroc_(&ONE, &ONE, &MYCOL, &ZERO, &MPIGRID[1]);
  std::vector<int> A(9), VECTOR(9);
  std::vector<double> A_mem(A_lrows*A_lcols), VECTOR_mem(VECTOR_lrows * VECTOR_lcols, 0);

  descinit_(A.data(), &N, &N, &NB, &NB, &ZERO, &ZERO,
            &BLACS_CONTEXT, &A_lrows, &info);
  descinit_(VECTOR.data(), &N, &ONE, &NB, &ONE, &ZERO, &ZERO,
            &BLACS_CONTEXT, &VECTOR_lrows, &info);

  std::mt19937 gen(MPIRANK);
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (int i = 0; i < A_lrows * A_lcols; ++i) {
    A_mem[i] = dist(gen);
  }
  // for (int i = 0; i < VECTOR_lrows; ++i) {

  // }

  std::vector<double> product(N * N, 0);

  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N,
              1.0,
              Q.data(), N, R.data(), N,
              0.0,
              product.data(), N);

  // pivot the original
  for (int j = 0; j < IPIV.size(); ++j) {
    for (int i = 0; i < N; ++i) {
      double t = original[i + (IPIV[j] - 1) * N];
      original[i + (IPIV[j] - 1) * N] = original[i + j * N];
      original[i + j * N] = t;
    }
  }

  double error = 0;
  for (int i = 0; i < A_lrows * A_lcols; ++i) {
    error += (original[i] - product[i]);
  }
  std::cout << "e: " << error << std::endl;

  MPI_Finalize();
}
