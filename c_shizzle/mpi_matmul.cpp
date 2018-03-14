#include "mpi.h"
#include <cstring>
#include <cmath>
#include <cblas.h>
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

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

// double frob_norm(double *A, int nrows, int ncols)
// {
//   double norm = 0;
//   for(size_t i=0; i < nrows*ncols; i++){
//     double val = A[i];
//     norm += val*val;
//   }
  
//   norm = sqrt(norm);
//   return norm;
// }

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

// void generate_matrix_a(double *mat, int nrows, int ncols)
// {
//   for (int i=0; i < nrows; i++)
//     for (int j = 0; j < ncols; j++)
//       mat[i*ncols j]= i+j;
// }

// void generate_matrix_b(double *mat, int nrows, int ncols)
// {
//     for (int i=0; i < nrows; i++)
//       for (int j = 0; j < ncols; j++)
//         mat[i*ncols j]= i+j;
// }

// void matmul_simple(double *a, double *b, double *c, int nrows_a, int ncols_a, int nrows_b, int ncols_b)
// {
//   cblas_dgemm(
//               CblasRowMajor, CblasNoTrans, CblasNoTrans,
//               nrows_a, ncols_b, nrows_b,
//               1, a, ncols_a, b,
//               ncols_b, 1, c, ncols_b);
// }

// // Subdivide the rows (vertical partitions) of the matrix b and keep a whole. Send each partition of b to a
// // different process. Send the whole of 'a' to each process.  The whole of A gets multiplied by the partition of b,
// // and then the respective process contains the mult of A with partition of B.
// //
// // There is redundant data copying of all elements of matrix A in this situation.
// void test_row_partition()
// {
//   int mpi_rank, mpi_size;
//   int nrows_a = 10, ncols_a = 8, nrows_b = 8, ncols_b = 10;
//   double *a, *b, *c;
//   int SENDING_A = 0;
//   MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
//   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

//   double *a = (double*)malloc(nrows_a*ncols_b*sizeof(double));
//   generate_matrix_a(a, nrows_a, ncols_a);

//   if (mpi_rank == MASTER) {
//     double *b = (double*)malloc(nrows_b*ncols_b*sizeof(double));
//     double *c = (double*)malloc(nrows_a*ncols_b*sizeof(double));
//     generate_matrix_b(b, nrows_b, ncols_b);
//   }

//   MPI_Barrier(MPI_COMM_WORLD);

//   if (mpi_rank == MASTER) { // master task jobs
//     // sending full matrix A
//     int parts = ncols_b / mpi_size;
//     double *temp = (double*)malloc(nrows_b*mpi_size*sizeof(double));
    
//     for (int i = 0; i < parts; ++i) {
//       // copy elements into a temp array
//       for (int j = 0; j < nrows_b; ++j) {
//         for (int k = 0; k < mpi_size; ++k) {
//           temp[j*mpi_size + k]; 
//         }
//       }
//       MPI_Send()
//     }
//   }
//   else { // worker jobs
//     // send partitions to workers.
//     // send full matrix A to workers.
//     // compute product between full A and part of B in worker.
//     // wait for output from processes.
//     // combine matrix B from processes.
//   }

//   MPI_Barrier(MPI_COMM_WORLD);

//   if (mpi_rank == MASTER) {
//     if (relative_error(c, answer, nrows_a, ncols_b) < 0.0001) {
//       printf("matmul row partition was a success.\n");
//     }
//   }
// }

// void test_simple_mult()
// {
//     double a[25] = {
//     1,2,3,4,5,
//     1,2,3,4,5,
//     1,2,3,4,5,
//     1,2,3,4,5
//   };
//   int nrows_a = 4, ncols_a = 5;
//   double b[25] = {
//     1,2,3,4,
//     1,2,3,4,
//     1,2,3,4,
//     1,2,3,4,
//     1,2,3,4
//   };
//   int nrows_b = 5, ncols_b = 4;
  
//   double d[20] = {
//     15,    30,    45,    60,
//     15,    30,    45,    60,
//     15,    30,    45,    60,
//     15,    30,    45,    60,
//   };
 
//   double *c = (double*)malloc(nrows_a*ncols_b*sizeof(double));
//   double start, end;
//   start = get_time();
//   matmul_simple(a, b, c, nrows_a, ncols_a, nrows_b, ncols_b);
//   end = get_time();
//   if (relative_error(c, answer, nrows_a, ncols_b) < 0.0001) {
//     printf("simple matmul was a success. time taken: %f\n", end - start);
//   }  
// }

void setup_processes(double *a, double *b, double *c, int nrows_a, int ncols_a, int nrows_b, int ncols_b)
{
  
}

void test_SUMMA()
{

  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if (mpi_rank == MASTER) {
    double start, end, total;
    double a[16] = {
      1,2,3,4,
      1,2,3,4,
      1,2,3,4,
      1,2,3,4
    };
    int nrows_a = 4, ncols_a = 4;

    double b[16] = {
      1,1,1,1,
      1,1,1,1,
      1,1,1,1,
      1,1,1,1
    };
    int nrows_b = 4, ncols_b = 4;

    double *c = (double*)calloc(16, sizeof(double));

    start = get_time();
    setup_processes(a, b, c, nrows_a, ncols_a, nrows_b, ncols_b);
    end = get_time();
    cout << "setup processes time: " << end - start << endl;
    // create a 4 x 4 processor grid and send one element to each.
  }
  else {
    summa_matmul();
  }


  for (int k = 0; k < 4; ++k) {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        c[i*4 + j] += a[i*4 + k]*b[k + j*4];
      }
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      cout << c[i*4 + j] << " ";
    }
    cout << std::endl;
  }

  //SUMMA();
}

int main(int c, char ** v)
{
  argc = c; argv = v;
  MPI_Init(&argc, &argv);
  //test_simple_mult();
  //test_row_partition();
  test_SUMMA();
  MPI_Finalize();
}
