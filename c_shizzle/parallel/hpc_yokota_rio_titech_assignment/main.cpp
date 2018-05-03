// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include <iostream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

extern "C" {
  void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K,
              double* ALPHA, double* A, int* LDA, double* B, int* LDB,
              double* BETA, double* C, int* LDC);
}

double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return double(tv.tv_sec)+double(tv.tv_usec)*1e-6;
}

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

void dgemm(double *A, double *B, double *C, int N, char T)
{
  double m1 = 1; double p1 = 1;
  int i;
  
  dgemm_(&T, &T, &N, &N, &N, &m1, A, &N, B, &N, &p1, C, &N);
}

void reset_matrix(double* C, int N, double val)
{
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      C[i*N + j] = val;
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "provide nrows and ncols" << endl;
    exit(1);
  }
  
  int N = atoi(argv[1]);
  double *A, *B, *C;
  A = (double*)calloc(sizeof(double), N*N);
  B = (double*)calloc(sizeof(double), N*N);
  C = (double*)calloc(sizeof(double), N*N);
  generate_data(A, B, C, N);
  
  double start = get_time();
  #pragma omp parallel for
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      for (int k=0; k<N; k++) {
        C[i*N + j] += A[i*N + k] * B[k*N + j];
      }
    }
  }
  double stop = get_time();

  cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << endl;

  double *D = (double*)calloc(sizeof(double), N*N);
  reset_matrix(D, N, 0);
  start = get_time();
  dgemm(A, B, D, N, 'T');
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
