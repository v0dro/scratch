// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include "config.hpp"

void generate_data(double *A, double* B, double *C, int N)
{
  for (int i=0; i < N; ++i) {
    for (int j=0; j < N; ++j) {
      A[i*N + j] = i*j + N + i*3;
      B[i*N + j] = i*j + N + j*4;
      C[i*N + j] = 0;
    }
  }
}

// performs element-wise multiplication
void macro_kernel(double *A, double *B, double *C, int i, int k)
{
  for (int j = 0; j < N; j += 1) {
    C[i*N + j] += A[i*N + k]*B[k*N + j];
  }
}

void macro_kernel_1x4(double *A, double *B, double *C, int i, int k)
{
  macro_kernel(A, B, C, i, k);
  macro_kernel(A, B, C, i+1, k);
  macro_kernel(A, B, C, i+2, k);
  macro_kernel(A, B, C, i+3, k);
}

// FIXME: implement proper packing logic
void packB_KCxNC(double *packB, int offsetb, double *B)
{
  
}

void superfast_matmul(double *A, double *B, double *C)
{  
  for (int i = 0; i < N; i += 4) {
    for (int k = 0; k < N; k += 1) { // advance rows of B.
      macro_kernel_1x4(A,B,C,i,k);
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "provide nrows and ncols" << endl;
    exit(1);
  }
  
  N = atoi(argv[1]); M = N;

  if (N % NC != 0) {
    cout << "N is not a multiple of NC.";
    exit(1);
  }
  
  double *A, *B, *C;
  double start, stop;
  A = (double*)calloc(sizeof(double), N*N);
  B = (double*)calloc(sizeof(double), N*N);
  C = (double*)calloc(sizeof(double), N*N);
  generate_data(A, B, C, N);
  
  start = get_time();
  superfast_matmul(A, B, C);
  stop = get_time();

  cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << endl;

  double *D = (double*)calloc(sizeof(double), N*N);
  reset_matrix(D, N, 0);
  start = get_time();
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      for (int k=0; k<N; k++) {
        D[i*N + j] += A[i*N + k] * B[k*N + j];
      }
    }
  }
  stop = get_time();

  cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << endl;


  double error = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      error += C[i*N + j] - D[i*N + j];
    }
  }

  cout << "error: " << error/N/N << endl;
  return 0;
}
