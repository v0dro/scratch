#include "mpi.h"
#include <cstdlib>
#include <iostream>
using namespace std;

// assume square block decomposition
int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  
  MPI_Finalize();
}
