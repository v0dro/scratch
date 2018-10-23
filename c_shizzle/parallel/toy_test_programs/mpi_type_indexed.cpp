// Sample program for demoing sending the lower triangle of a square
// matrix using types made by the MPI_Type_indexed function.

#include "mpi.h"
#include <iostream>
using namespace std;

int main()
{
  MPI_Init(NULL, NULL);
  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  double A[16], G[16];
  int displs[4] = {0, 4, 8, 12};
  int lens[4] = {1, 2, 3, 4};
  MPI_Datatype tril;
  MPI_Status status;

  for (int i = 0; i < 16; i++)
    A[i] = i+1;

  MPI_Type_indexed(4, lens, displs, MPI_DOUBLE, &tril);
  MPI_Type_commit(&tril);

  if (mpi_rank == 0) {
    MPI_Send(A, 1, tril, 1, 0, MPI_COMM_WORLD);
  }

  if (mpi_rank == 1) {
    MPI_Recv(G, 1, tril, 0, 0, MPI_COMM_WORLD, &status);

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (j <= i)
          cout << G[i*4 + j] << " ";
        else
          cout << " ";
      }
      cout << endl;
    }
  }
  MPI_Type_free(&tril);
  MPI_Finalize();
}
