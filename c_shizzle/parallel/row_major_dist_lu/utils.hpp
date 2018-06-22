#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
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
  int myrow;
  int mycol;
  int num_procs;
} mpi_desc;

void initialize_blacs(int *BLACS_CONTEXT, int *proc_nrows, int *proc_ncols,
                      int *myrow, int *mycol, int *proc_id, int *num_procs);

void generate_data(double *a, int myrow, int mycol, desc desc_a);

void print_arr(double *A, int size, string desc, ostream &o);

void print_mat(double *A, int nrows, int ncols, int myrow, int mycol,
               string desc, ostream &o);
  
void print_all(double *A, int nrows, int ncols, int myrow, int mycol, char *desc);

void print_block(double *A, int nb, int nc, int myrow, int mycol, char *desc);

void print_files(double *A, int nrows, int ncols, int myrow, int mycol, string postfix="");

void local2global(int *local, int *global, int myrow, int mycol);

void global2local(int global, int *local, int num_procs, desc desc_a);
#endif /* UTILS_H */
