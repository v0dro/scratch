#include <iostream>
#include <cmath>
#include <iomanip>
#include "mpi.h"

int MPISIZE, MPIRANK;
int MPIGRID[2];

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);

  int N = atoi(argv[1]);
  int NB = atoi(argv[2]);

  int nblocks = N / NB;

  double *A = new double[N * N]();
  double *B = new double[NB * NB]();

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      A[i + j * N] = i * N + j;
    }
  }

  if (MPIRANK == 0) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        std::cout << std::setw(5) << A[i + j * N] << " ";
      }
      std::cout << std::endl;
    }
  }


  MPI_Datatype SUBMATRIX;
  // https://www.mpich.org/static/docs/v3.3/www3/MPI_Type_vector.html
  MPI_Type_vector(NB, NB, N, MPI_DOUBLE, &SUBMATRIX);

  MPI_Datatype BOTTOM_RIGHT;
  // const int array_of_sizes[2] = {N, N};
  // const int array_of_subsizes[2] = {NB, NB};
  // const int array_of_starts[2] = {NB * (nblocks-1), NB * (nblocks-1)};
  // MPI_Type_create_subarray(2,
  //                          array_of_sizes,
  //                          array_of_subsizes,
  //                          array_of_starts,
  //                          MPI_ORDER_FORTRAN,
  //                          MPI_DOUBLE,
  //                          &BOTTOM_RIGHT);

  MPI_Type_create_resized(SUBMATRIX,
                          NB * sizeof(double),
                          N * sizeof(double),
                          &BOTTOM_RIGHT);
  MPI_Type_commit(&BOTTOM_RIGHT);

  MPI_Datatype SMALL_TILE;
  MPI_Type_contiguous(NB * NB, MPI_DOUBLE, &SMALL_TILE);
  MPI_Type_commit(&SMALL_TILE);

  if (MPIRANK == 0) {
    MPI_Send(A, 1, BOTTOM_RIGHT, 1, 1, MPI_COMM_WORLD);
  }

  if (MPIRANK == 1) {
    MPI_Status status;
    MPI_Recv(B, 1, SMALL_TILE, 0, 1, MPI_COMM_WORLD, &status);
  }

  if (MPIRANK == 1) {
    for (int i = 0; i < NB; ++i) {
      for (int j = 0; j < NB; ++j) {
        std::cout << std::setw(5) << B[i + j * NB] << " ";
      }
      std::cout << std::endl;
    }
  }


  delete[] A;
  delete[] B;
  MPI_Finalize();
}
