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

  // matrix properties
  int N = 8, nb = 4; // mat size, blk size.
  int nelements; // number of elements in each process.
  int* data = (int*)malloc(sizeof(int)*nb*nb);
  // end matrix properties

  // BLACS init
  int BLACS_CONTEXT, proc_nrows, proc_ncols, myrow, mycol;
  // int info, itemp;
  int proc_id, num_procs;
  proc_nrows = 2; proc_ncols = 2;
  int proc_dims[2] = {proc_nrows, proc_ncols};
  Cblacs_pinfo(&proc_id, &num_procs);
  Cblacs_get( -1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit( &BLACS_CONTEXT, "Row", proc_nrows, proc_ncols );
  Cblacs_pcoord(BLACS_CONTEXT, mpi_rank, &myrow, &mycol);
  cout << "myrow " << myrow << " mycol " << mycol << endl;
  cout << "procid " << proc_id << " num_procs " << num_procs << endl;
  // end BLACS init

  // MPI IO read
  MPI_Status status;
  MPI_File file;
  // MPI_Offset offset;
  MPI_Datatype MPI_darray;
  int darray_size;
  int dims[2] = {N, N};
  int distribs[2] = {MPI_DISTRIBUTE_CYCLIC, MPI_DISTRIBUTE_CYCLIC};
  int dargs[2] = {nb, nb};
  MPI_Aint lower_bound, darray_extent;

  // MPI darray type create
  MPI_Type_create_darray(num_procs, proc_id, 2, dims, distribs, dargs,
                         proc_dims, MPI_ORDER_C, MPI_INT, &MPI_darray);
  MPI_Type_commit(&MPI_darray);
  MPI_Type_size(MPI_darray, &darray_size);
  nelements = darray_size / 4;
  cout << "nelements " << nelements << endl;
  MPI_Type_get_extent(MPI_darray, &lower_bound, &darray_extent);
  // end MPI darray type create

  // read file
  MPI_Offset displacement = 0;
  MPI_File_open(MPI_COMM_WORLD, "data.dat", MPI_MODE_RDONLY,
                MPI_INFO_NULL, &file);
  MPI_File_set_view(file, displacement, MPI_INT, MPI_darray, "native", MPI_INFO_NULL);
  MPI_File_read_all(file, data, nelements, MPI_INT, &status);
  MPI_File_close(&file);
  // end read file

  for (int i = 0; i < nelements; ++i) {
    cout << "r: " << myrow << " c: " << mycol << " " << data[i] << endl;
  }
 
  // end MPI IO read

  MPI_Finalize();
}
