// Basic program to demo P2P communication between processes in a grid using BLACS.

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
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

  void descinit_(int *desc, const int *m,  const int *n, const int *mb, 
                 const int *nb, const int *irsrc, const int *icsrc, const int *ictxt, 
                 const int *lld, int *info);

  void Cdgesd2d(
                int CBLACS_CONTEXT, // CBLACS context
                int M, // row size of matrix block
                int N, // col size of matrix block
                double* A, // pointer to matrix block
                int LDA, // leading dim of A (col size for C programs)
                int RDEST, // row number of destination process
                int CDEST // col number of destination process
                );
  
  void Cdgerv2d(
                int CBLACS_CONTEXT, // CBLACS context
                int M, // row size of matrix block
                int N, // col size of matrix block
                double *A, // pointer to matrix data.
                int LDA, // leading dim of A (col size for C)
                int RSRC, // process row co-ordinate of the sending process.
                int CSRC // process col co-ordinate of the sending process.
                );
}

// struct defining properties of an MPI process.
typedef struct mpi_desc {
  int myrow;     // current process row number.
  int mycol;     // current process column number.
  int proc_id;
  int BLACS_CONTEXT;
  int num_procs; // total number of processes.
  int MP;        // no. of rows of process grid.
  int NP;        // no. of cols of process grid.
} mpi_desc;

void initialize_blacs(int *BLACS_CONTEXT, int *proc_nrows, int *proc_ncols,
                      int *myrow, int *mycol, int *proc_id, int *num_procs)
{
  *proc_nrows = 2; *proc_ncols = 2;
  Cblacs_pinfo(proc_id, num_procs);
  Cblacs_get( -1, 0, BLACS_CONTEXT );
  Cblacs_gridinit( BLACS_CONTEXT, "Row", *proc_nrows, *proc_ncols );
  Cblacs_pcoord(*BLACS_CONTEXT, *proc_id, myrow, mycol);
}

int main()
{
  mpi_desc mpi;
  initialize_blacs(&mpi.BLACS_CONTEXT, &mpi.MP, &mpi.NP,
                   &mpi.myrow, &mpi.mycol, &mpi.proc_id,
                   &mpi.num_procs);
  ofstream myfile;
  myfile.open ("p2p_send_recv_op.txt");

  int size = mpi.MP;
  double max[mpi.MP*size];
  max[mpi.myrow*size] = mpi.myrow + 2;
  max[mpi.myrow*size + 1] = mpi.myrow + 2 + 4;

  // Send the local imax and vmax to all processes in the same column:
  if (mpi.mycol == 0) {
    for (int r = 0; r < mpi.MP; r++) {
      if (mpi.myrow == 0 && mpi.mycol == 0)
        myfile << "sending from (" << r << "," << mpi.mycol << ") max array : " << max[0] << " " << max[1] << " " << max[2] << " " << max[3] << endl; 
      Cdgesd2d(mpi.BLACS_CONTEXT, 2, 1, &max[mpi.myrow*size], 1, r, mpi.mycol);
    }

    //}
    // Receive the imax and vmax of all processes in the same column:
    for (int r = 0; r < mpi.MP; r++) {
      if (mpi.myrow == 1 && mpi.mycol == 0)
        myfile << "receiveing at (" << r << ", " << mpi.mycol << ") max array : " << max[0] << " " << max[1] << " " << max[2] << " " << max[3] << endl;
      Cdgerv2d(mpi.BLACS_CONTEXT, 2, 1, &max[r*size], 1, r, mpi.mycol);
    }

    if (mpi.myrow == 1 && mpi.mycol == 0) {
      myfile << "max array : " << mpi.MP << endl;
      for (int i = 0; i < mpi.MP; i++) {
        myfile << "max " << max[i*size] << " " << max[i*size + 1] << endl; 
      }
    }
  }
  myfile.close();
}
