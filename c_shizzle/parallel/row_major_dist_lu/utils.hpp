#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
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
  
}

void initialize_blacs(int *BLACS_CONTEXT, int *proc_nrows, int *proc_ncols,
                      int *myrow, int *mycol, int *proc_id, int *num_procs);

void generate_data(double *a, int block_size_per_process_r,
                   int block_size_per_process_c, int process_block_size,
                   int num_blocks_per_process, int myrow, int mycol, int N);

void print_arr(double *A, int size, string desc, ostream &o);

void print_mat(double *A, int nrows, int ncols, int myrow, int mycol,
               string desc, ostream &o);
  
void print_all(double *A, int nrows, int ncols, int myrow, int mycol, char *desc);

void print_block(double *A, int nb, int nc, int myrow, int mycol, char *desc);

void print_files(double *A, int nrows, int ncols, int myrow, int mycol, string postfix="");
#endif /* UTILS_H */
