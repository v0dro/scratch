#include "mpi.h"
#include <cstring>
#include <cmath>
#include <cblas.h>
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <fstream>

using namespace std;
#define MASTER 0

int argc;
char ** argv;

double get_time()
{
  struct timeval t;
  struct timezone tzp;
  gettimeofday(&t, &tzp);
  return t.tv_sec + t.tv_usec*1e-6;
}

// double relative_error(double *A, double *B, int nrows, int ncols)
// {
//   int i;
//   double *A_minus_B = (double*)malloc(sizeof(double)*nrows*ncols);
//   for (i = 0; i < nrows*ncols; ++i) {
//     A_minus_B[i] = A[i] - B[i];
//   }
//   double normA = frob_norm(A, nrows, ncols);
//   double normA_minus_B = frob_norm(A_minus_B, nrows, ncols);
//   return normA_minus_B/normA;
// }


void matmul(double *a, double *b, double *c, int nrows_a, int ncols_a, int nrows_b, int ncols_b)
{
  cblas_dgemm(
              CblasRowMajor, CblasNoTrans, CblasNoTrans,
              nrows_a, ncols_b, nrows_b,
              1, a, ncols_a, b,
              ncols_b, 1, c, ncols_b);
}


void generate_rand_array(double *x, int nrows) {
  srand(time(NULL));
  for (int i = 0; i < nrows; ++i) {
    x[i] = (double)rand();
  }
}

void reconstruct_matrix(double *c, int nrows, int ncols, int mpi_rank)
{
  MPI_File file;
  MPI_Offset offset = nrows*ncols*mpi_rank*sizeof(double);
  MPI_Status s;

  MPI_File_open(MPI_COMM_WORLD, "data.out", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
  MPI_File_seek(file, offset, MPI_SEEK_SET);
  MPI_File_write(file, c, nrows*ncols, MPI_DOUBLE, &s);
  MPI_File_close(&file);
}

double frob_norm(double *A, int nrows, int ncols)
{
  double norm = 0;
  for(size_t i=0; i < nrows*ncols; i++){
    double val = A[i];
    norm += val*val;
  }
  
  norm = sqrt(norm);
  return norm;
}

void read_mat(int NROWS, int NCOLS)
{
  MPI_File fh; MPI_Status s;
  double c[NROWS*NCOLS];
  int status;
  
  status = MPI_File_open(MPI_COMM_WORLD, "data.out", MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
  MPI_File_read_all(fh, c, NROWS*NCOLS, MPI_DOUBLE, MPI_STATUS_IGNORE);
  MPI_File_close(&fh);
  cout << "norm: " << frob_norm(c, NROWS, NCOLS) << endl;
}

void summa(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  
  int N = sqrt(mpi_size), NROWS = 100, NCOLS = 100;
  double *a; double *b; double *c;
  int row_rank = mpi_rank / N;
  int col_rank = mpi_rank % N;
  int nrows = NROWS/N;
  int ncols = NCOLS/N;
  MPI_Request req[2];

  double x[NROWS];
  generate_rand_array(x, NROWS);

  a = (double*)malloc(nrows*ncols*sizeof(double));
  b = (double*)malloc(nrows*ncols*sizeof(double));
  c = (double*)calloc(nrows*ncols, sizeof(double));
  
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      a[i*nrows + j] = 1; /// abs(x[i + col_rank*ncols] - x[j + col_rank*ncols] - ncols);
      b[i*nrows + j] = 1;
    }
  }

  // send a to all cols in a given row.
  for (int c = 0; c < N; ++c) {
    int proc_number = row_rank*N + c;
    MPI_Isend(a, nrows*ncols, MPI_DOUBLE, proc_number, 0, MPI_COMM_WORLD, &req[0]);
  }

  // send b to all rows in a given col.
  for (int r = 0; r < N; ++r) {
    int proc_number = r*N + col_rank;
    MPI_Isend(b, nrows*ncols, MPI_DOUBLE, proc_number, 1, MPI_COMM_WORLD, &req[1]);
  }

  MPI_Status s;

  // get numbers from both rows and columns and add them to the product.
  for (int i = 0; i < N; ++i) {
    int proc_number_r = row_rank*N + i;
    int proc_number_c = i*N + col_rank;
    MPI_Recv(a, nrows*ncols, MPI_DOUBLE, proc_number_r, 0, MPI_COMM_WORLD, &s);
    MPI_Recv(b, nrows*ncols, MPI_DOUBLE, proc_number_c, 1, MPI_COMM_WORLD, &s);
    matmul(a, b, c, nrows, ncols, nrows, ncols);
  }

  cout << "r: " << row_rank << " c: " << col_rank << " rank: " << mpi_rank << " norm of c: " << frob_norm(c, nrows, ncols) << endl;

  MPI_Barrier(MPI_COMM_WORLD);
  reconstruct_matrix(c, nrows, ncols, mpi_rank);
  MPI_Barrier(MPI_COMM_WORLD);
  read_mat(NROWS, NCOLS);
  
  MPI_Finalize();
}

int main(int c, char ** v)
{
  argc = c; argv = v;
  summa(argc, argv);
}
