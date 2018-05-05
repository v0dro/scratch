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
inline void macro_kernel(double *A, double *B, double *C, int i, int j, int k)
{
  for (int a = k; a < KC + k; a++) {
    for (int b = j; b < MC + j; b++) {
      C[i*N + b] += A[i*N + a] * B[a*N + b];
    }
  }
  // double *A_ptr, *B_ptr, *C_ptr;

  // A_ptr = &A[i*N + k];
  // B_ptr = &B[k*N + j];
  // C_ptr = &C[i*N + j];
        
  // *C_ptr += (*A_ptr) * (*B_ptr);
  // *(C_ptr+1) += (*A_ptr) * (*(B_ptr+1));
  // *(C_ptr+2) += (*A_ptr) * (*(B_ptr+2));
  // *(C_ptr+3) += (*A_ptr) * (*(B_ptr+3));
  // *(C_ptr+4) += (*A_ptr) * (*(B_ptr+4));  
}

void  superfast_matmul(double *A, double *B, double *C)
{ 
  for (int i = 0; i < N; i += NC) {
    for (int k = 0; k < N; k += KC) { // advance by cache block size for col
      // pack B into an array of size Mc X N.
      // each subarray of size Mc X Nr is packed separately.
      // pack into an array Bc.
      for (int j = 0; j < N; j += MC) { // advance by cache block size for row
        // pack A into an array of size MC X KC
        // each horizontal subarray is of size MR X KC.
        // each MR X KC is packed separately.
        macro_kernel(A, B, C, i, j, k);
      }
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
