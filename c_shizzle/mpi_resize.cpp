#include <iostream>
#include "mpi.h"

int MPISIZE, MPIRANK;
int MPIGRID[2];

int main(int argc, char** argv) {
  MPI_Init(argc, argv);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);

  int N = atoi(argv[1]);
  int NB = atoi(argv[2]);

  double *A = new double[N * N];
  double *B = new double[NB * NB];



  delete[] A;
  MPI_Finalize();
}
