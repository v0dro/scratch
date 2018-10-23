#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  double f, d;
  MPI_Request req_s, req_r, req_r1;
  MPI_Status s, s1;

  if (rank == 0) {
    f = 36;
    sleep(3);
    MPI_Isend(&f, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req_s);
  }

  if (rank == 2) {
    f = 48;
    MPI_Isend(&f, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &req_s);
  }

  if (rank == 1) {
    MPI_Irecv(&f, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req_r);
    //MPI_Wait(&req_r, &s);

    printf("reached below.\n");

    MPI_Irecv(&d, 1, MPI_DOUBLE, 2, 1, MPI_COMM_WORLD, &req_r1);
    MPI_Wait(&req_r1, &s1);
    MPI_Wait(&req_r, &s);
    
    printf("received f: %f.\n", f);
    printf("received d: %f.\n", d);
  }

  MPI_Finalize();
  return 0;
}
