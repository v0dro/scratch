#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include "mpi.h"
#include <sys/time.h>
#include <omp.h>
using namespace std;

double timeofday() {
  timeval t;
  gettimeofday(&t, NULL);
  
  return t.tv_sec + t.tv_usec * 1e-6;
}

int main(int argc, char *argv[])
{
  int ROWRANK, COLRANK;
  int MPIRANK, MPISIZE;
  int PNROWS = 2, PNCOLS = 2;
  int N = 500;
  int NB = N / PNROWS;
  double start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);

  assert(PNROWS * PNCOLS == MPISIZE);

  ROWRANK = (int)(MPIRANK / PNCOLS);
  COLRANK = MPIRANK % PNCOLS;

  std::vector<double> A(NB*NB, 2);
  std::vector<double> B(NB, 1);
  std::vector<double> C(NB, 0);

  int LDAB = NB;
  int LDBB = 1;
  int LDCB = 1;

  // Multiply own block and send async to all procs along same column.

  start = timeofday();
  // Multiply own blocks
  //  #pragma omp parallel for
  for (int i = 0; i < NB; ++i) { // m
    for (int j = 0; j < 1; ++j) { // n
      //  #pragma omp parallel for
      for (int k = 0; k < NB; ++k) { // k
        C[i * LDCB + j] += A[i * LDAB + k] * B[k * LDBB + j];
      }
    }
  }

  MPI_Request req[PNROWS];
  
  // Send along cols
  for (int i = 0; i < PNROWS; ++i) {
    if (ROWRANK != i) {
      // std::cout << "ROWRANK: " << ROWRANK << " COLRANK: " << COLRANK << " MPIRANK: "
      //           << MPIRANK  << " sending to... " << i * PNROWS + COLRANK << std::endl; 
      MPI_Isend(&C[0], NB, MPI_DOUBLE, i * PNROWS + COLRANK, MPIRANK, MPI_COMM_WORLD, &req[i]);
    }
  }

  for (int i = 0; i < PNROWS; ++i) {
    if (ROWRANK != i) {
      //      std::cout << "recv from " << i * PNROWS + COLRANK << std::endl;
      double recv[NB];
      MPI_Status s;
      MPI_Recv(recv, NB, MPI_DOUBLE, i * PNROWS + COLRANK, i * PNROWS + COLRANK, MPI_COMM_WORLD, &s);

      //      #pragma omp parallel for
      for (int i = 0; i < NB; ++i) {
        C[i] += recv[i];
      }
    }
  }

  stop = timeofday();

  // if (COLRANK == 0) {
  //   std::cout << "MPIRANK: " << MPIRANK << std::endl;
  //   std::cout << "------------------C-------------------\n";
  //   for (int i = 0; i < NB; ++i) {
  //     std::cout << C[i] << std::endl;
  //   }
  //   std::cout << std::endl;
  // }

  std::cout << "TIME::" << stop - start << std::endl;
  
  MPI_Finalize();
  
  return 0;
}
