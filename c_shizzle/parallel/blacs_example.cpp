#include "mpi.h"
#include <iostream>
using namespace std;

extern "C" {
  /* Cblacs declarations */
  void Cblacs_pinfo(int*, int*);
  void Cblacs_get(int, int, int*);
  void Cblacs_gridinit(int*, const char*, int, int);
  void Cblacs_pcoord(int, int, int*, int*);
  void Cblacs_gridexit(int);
  void Cblacs_barrier(int, const char*);
  void Cdgerv2d(int, int, int, double*, int, int, int);
  void Cdgesd2d(int, int, int, double*, int, int, int);
 
  int numroc_(int*, int*, int*, int*, int*);
}

int main(int argc, char ** argv)
{
  int NROWS = 8, NCOLS = 8;
  int B_NROWS = 2, B_NCOLS = 2;
  int mpi_rank, mpi_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  // BLACS stuff
  int proc_id, proc_row, proc_col, BLACS_CONTEXT, num_procs;
  int proc_nrows = 2, proc_ncols = 2; // 2x2 process grid
  Cblacs_pinfo(&proc_id, &num_procs);
  Cblacs_get(0,0,&BLACS_CONTEXT);
  Cblacs_gridinit(&BLACS_CONTEXT, "Row-major", proc_nrows, proc_ncols);
  Cblacs_pcoord(BLACS_CONTEXT, proc_id, &proc_row, &proc_col); // get grid coords.

  cout << "row: " << proc_row << "col: " << proc_col << endl;
  
  MPI_Finalize();  
}
