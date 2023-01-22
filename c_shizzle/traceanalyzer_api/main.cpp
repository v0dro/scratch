#include "mpi.h"
#include "VT.h"

int MPISIZE, MPIRANK;
int MPIGRID[2];

int main() {
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Dims_create(MPISIZE, 2, MPIGRID);

  MPI_Finalize();

  return 0;
}
