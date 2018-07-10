#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

#define ROW_MAJOR 1
#define COL_MAJOR NULL

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

  void Cdgebr2d(
                int CBLACS_CONTEXT, // CBLACS context
                char* SCOPE, // scope of the broadcast. Can be "Row", "Column" or "All"
                char* TOP, // indicates communication pattern to use for broadcast.
                int M, // number of rows of matrix.
                int N, // number of columns of matrix.
                double* A, // pointer to matrix data.
                int LDA, // leading dim of matrix (col size for C)
                int RSRC, // process row co-ordinate of the process who called broadcast/send.
                int CSRC // process column co-ordinate of the process who called broadcast/send.
                );

  void Cdgebs2d(
                int CBLACS_CONTEXT, // CBLACS context.
                char* SCOPE, // scope of broadcast. can be "All", "Row" or "Column".
                char* TOP, // network topology to be used.
                int M, // num of rows of the matrix.
                int N, // num of cols of the matrix.
                double *A, // pointer to the matrix data.
                int LDA // leading dimension of A.
                );
  
  void pdgemm_( char* TRANSA, char* TRANSB,
                int * M, int * N, int * K,
                double * ALPHA,
                double * A, int * IA, int * JA, int * DESCA,
                double * B, int * IB, int * JB, int * DESCB,
                double * BETA,
                double * C, int * IC, int * JC, int * DESCC );

  void pdgetrf_(int * M, int * N, double * A,
                int * IA, int * JA, int * DESCA,
                int * IPIV, int * INFO);

  void pdgetf2_(int * M, int * N, double * A,
                int * IA, int * JA, int * DESCA, int *IPIV,
                int * INFO);

  void pdamax_( int * N, double * AMAX, int * INDX,
                double * X, int * IX, int * JX,
                int * DESCX, int * INCX );
  
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
  int proc_id;
  int BLACS_CONTEXT;
  int num_procs; // total number of processes.
  int MP;        // no. of rows of process grid.
  int NP;        // no. of cols of process grid.
} mpi_desc;

extern ofstream err_file;

void initialize_blacs(int *BLACS_CONTEXT, int *proc_nrows, int *proc_ncols,
                      int *myrow, int *mycol, int *proc_id, int *num_procs);

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
#endif /* UTILS_H */
