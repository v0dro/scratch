#include <iostream>
#include <cmath>
#include <iomanip>
#include "mpi.h"

extern "C" {
  int ompi_datatype_dump(MPI_Datatype type);
}

int MPI_Pack(const void *inbuf,
             int incount,
             MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm);

int MPISIZE, MPIRANK;
int MPIGRID[2];

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);

  int nleaf = atoi(argv[1]);
  int rank = atoi(argv[2]);

  double *A = new double[nleaf * nleaf]();
  double *B = new double[rank * rank]();

  for (int i = 0; i < nleaf; ++i) {
    for (int j = 0; j < nleaf; ++j) {
      A[i + j * nleaf] = i * nleaf + j;
    }
  }

  for (int i = 0; i < nleaf; ++i) {
    for (int j = 0; j < nleaf; ++j) {
      std::cout << std::setw(5) << A[i + j * nleaf] << " ";
    }
    std::cout << std::endl;
  }



  MPI_Datatype SUBMATRIX;
  // https://www.mpich.org/static/docs/v3.3/www3/MPI_Type_vector.html
  MPI_Type_vector(rank, rank, nleaf, MPI_DOUBLE, &SUBMATRIX);

  MPI_Datatype BOTTOM_RIGHT;
  const int array_of_sizes[2] = {N, N};
  const int array_of_subsizes[2] = {rank, rank};
  const int array_of_starts[2] = {rank * (nblocks-1), rank * (nblocks-1)};
  MPI_Type_create_subarray(2,
                           array_of_sizes,
                           array_of_subsizes,
                           array_of_starts,
                           MPI_ORDER_FORTRAN,
                           MPI_DOUBLE,
                           &BOTTOM_RIGHT);

  // MPI_Type_create_resized(SUBMATRIX,
  //                         ((nleaf - rank) * nleaf + (nleaf - rank)) * sizeof(double),
  //                         rank * rank * sizeof(double),
  //                         &BOTTOM_RIGHT);
  MPI_Type_commit(&BOTTOM_RIGHT);

  ompi_datatype_dump(BOTTOM_RIGHT);

  MPI_Datatype SMALL_TILE;
  MPI_Type_contiguous(rank * rank, MPI_DOUBLE, &SMALL_TILE);
  MPI_Type_commit(&SMALL_TILE);


  MPI_Request req;
  MPI_Irecv(B, 1, SMALL_TILE, 0, 0, MPI_COMM_SELF, &req);
  MPI_Send(A, 1, BOTTOM_RIGHT, 0, 0, MPI_COMM_SELF);
  MPI_Wait(&req, MPI_STATUS_IGNORE);


    for (int i = 0; i < rank; ++i) {
      for (int j = 0; j < rank; ++j) {
        std::cout << std::setw(5) << B[i + j * rank] << " ";
      }
      std::cout << std::endl;
    }
  delete[] A;
  delete[] B;
  MPI_Finalize();
}
