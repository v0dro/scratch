#ifndef UTILS_H
#define UTILS_H

#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

#define ROW_MAJOR 1
#define COL_MAJOR NULL

extern "C" { 
  int numroc_(int*, int*, int*, int*, int*);

  void descinit_(int *desc, const int *m,  const int *n, const int *mb, 
    const int *nb, const int *irsrc, const int *icsrc, const int *ictxt, 
    const int *lld, int *info);


  int MPI_Send(
               const void* data,
               int count,
               MPI_Datatype datatype,
               int destination,
               int tag,
               MPI_Comm communicator);

  int MPI_Recv(
               void* data,
               int count,
               MPI_Datatype datatype,
               int source,
               int tag,
               MPI_Comm communicator,
               MPI_Status* status);
}

// Descriptor of a matrix.
typedef struct desc {
  int M;             // numrows of global matrix A
  int N;             // numcols of global matrix A
  int MB;            // numrows of blocks of matrix A
  int NB;            // numcols of blocks of matrix A
  int BLACS_CONTEXT; // Blacs context handle
  int rsrc;          // The process row number containing the first row of the global matrix A
  int csrc;          // The process col containing the first col of the global matrix A
  int lld;           // Leading dimension of the number of the local array
} desc;

// struct defining properties of an MPI process.
typedef struct mpi_desc {
  int myrow;     // current process row number.
  int mycol;     // current process column number.
  int proc_id;   // process ID
  int BLACS_CONTEXT;
  int num_procs; // total number of processes.
  int MP;        // no. of rows of process grid.
  int NP;        // no. of cols of process grid.
  MPI_Comm comm; // MPI communicator
} mpi_desc;

extern ofstream err_file;

void init_mpi(int *myrow, int *mycol, int *proc_id, int *num_procs,
              int *proc_nrows, int *proc_ncols);

void generate_data(double *a, int myrow, int mycol, desc desc_a);

void print_arr(double *A, int size, string desc, ostream &o);

void print_mat(double *A, int nrows, int ncols, int myrow, int mycol,
               string desc, ostream &o);
  
void print_all(double *A, int nrows, int ncols, int myrow, int mycol, char *desc);

void print_block(double *A, int nb, int nc, int myrow, int mycol, char *desc);

void print_files(double *A, int nrows, int ncols, int myrow, int mycol, string postfix="");

void index2coords(int index, int ld, int &row, int &col);

void procg2l(int grow, int gcol, int *newrow, int *newcol, desc desc_a, mpi_desc mpi);
  
void local2global(int local, int *global, int myrow, int mycol, int num_procs, desc desc_a);

void global2local(int global, int *local, int num_procs, desc desc_a);

void g2l(int grow, int gcol, int &lrow, int &lcol, desc desc_a, mpi_desc mpi);

void l2g(int lrow, int lcol, int &grow, int &gcol, desc desc_a, mpi_desc mpi);

void mat_block(int grow, int gcol, int &mb_row, int &mb_col, desc desc_a);

void init_mpi(int *myrow, int *mycol, int *proc_id, int *num_procs,
              int *proc_nrows, int *proc_ncols);

int send(void* data, int dest_row, int dest_col, int count, int tag,
         MPI_Datatype type, mpi_desc mpi);

int recv(void *data, int srow, int scol, int count, int tag,
         MPI_Datatype type, mpi_desc mpi, MPI_Status* status);
#endif /* UTILS_H */
