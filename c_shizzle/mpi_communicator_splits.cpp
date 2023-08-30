#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  int MPIRANK, MPISIZE;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);

  MPI_Group world_group;
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);

  int ranks[2] = {0, 2};
  MPI_Group new_grp;
  MPI_Group_incl(world_group, 2, ranks, &new_grp);

  MPI_Comm new_comm;
  MPI_Comm_create(MPI_COMM_WORLD, new_grp, &new_comm);

  int value = -99;

  if (MPIRANK == 0 || MPIRANK == 2) {
    value = 44;
  }

  int new_value = -11;

  if (new_comm != MPI_COMM_NULL) {
    MPI_Allreduce(&value, &new_value, 1, MPI_INT, MPI_SUM, new_comm);
  }

  std::cout << "rank: " << MPIRANK << " value: " << new_value << std::endl;

  MPI_Finalize();
}
