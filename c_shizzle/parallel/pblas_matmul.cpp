#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

extern "C" {
  /* Cblacs declarations */
  void Cblacs_pinfo(int*, int*);
  void Cblacs_get(int, int, int*);
  void Cblacs_gridinit(int*, const char*, int, int);
  void Cblacs_pcoord(int, int, int*, int*);
  void Cblacs_gridexit(int);
  void Cblacs_barrier(int, const char*);
 
  int numroc_(int*, int*, int*, int*, int*);
}


int main(int argc, char ** argv)
{
  // MPI init
  MPI_Init(&argc, &argv);
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  // end MPI init

    // MPI IO read
  MPI_Status status;
  MPI_File file;
  MPI_Offset offset;

  MPI_File_open(MPI_COMM_WORLD, "data.out", MPI_MODE_RDONLY,
                MPI_INFO_NULL, &file);
  
  // MPI IO read end

  
  // BLACS init
  int BLACS_CONTEXT, proc_nrows, proc_ncols, myrow, mycol, nb;
  int info, itemp;
  int proc_id, num_procs;
  int ZERO=0,ONE=1;
  proc_nrows = 3; proc_ncols = 3; nb =2;
  Cblacs_pinfo(&proc_id, &num_procs);
  Cblacs_get( -1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit( &BLACS_CONTEXT, "Row", proc_nrows, proc_ncols );
  Cblacs_pcoord(BLACS_CONTEXT, mpi_rank, &myrow, &mycol);
  cout << "myrow " << myrow << " mycol " << mycol << endl;
  // BLACS init end
  
  MPI_Finalize();
}
