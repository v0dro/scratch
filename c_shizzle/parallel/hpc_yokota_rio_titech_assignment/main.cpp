// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include "config.hpp"

#define A(i,j) A[(i)*N + j]
#define B(i,j) B[(i)*N + j]
#define C(i,j) C[(i)*N + j]
#define BLOCK 4

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
  register double a0 = A(0,k), a1 = A(1,k), a2 = A(2,k), a3 = A(3,k);
  double *b_ptr = &B(k,0);
  
  for (int j = 0; j < N; j += 1) {
    C(0,j) += a0*(*b_ptr);
    C(1,j) += a1*(*b_ptr);
    C(2,j) += a2*(*b_ptr);
    C(3,j) += a3*(*b_ptr++);
  }
}

void macro_kernel_4x4(double *A, double *B, double *C, int i, int k)
{
  // register double a0 = A(0,k), a1 = A(1,k), a2 = A(2,k), a3 = A(3,k);
  // double *b_ptr = &B(k,0);
  
  for (int j = 0; j < N; j += 1) {

// -    for (int ii = i; ii < BLOCK+i; ++ii) {
//       for (int kk = k; kk < BLOCK+k; ++kk) {
//         for (int jj = j; jj < BLOCK+j; ++jj) {
//           C(ii,jj) += A(ii,kk)*B(kk,jj);
//         }
//       }
//     }
//    for (int ii=i; ii < BLOCK+i; ++ii) {
      C(i,j) += A(i,k)*B(k,j);
      C(i,j) += A(i,k+1)*B(k+1,j);
      C(i,j) += A(i,k+2)*B(k+2,j);
      C(i,j) += A(i,k+3)*B(k+3,j);

      C(i+1,j)   += A(i+1,k)*B(k,j);
      C(i+1,j) += A(i+2,k+1)*B(k+1,j);
      C(i+1,j) += A(i+2,k+2)*B(k+2,j);
      C(i+1,j) += A(i+2,k+3)*B(k+3,j);

      C(i+2,j)   += A(i+2,k)*B(k,j);
      C(i+2,j) += A(i+2,k+1)*B(k+1,j);
      C(i+2,j) += A(i+2,k+2)*B(k+2,j);
      C(i+2,j) += A(i+2,k+3)*B(k+3,j);

      C(i+3,j) += A(i+3,k)*B(k,j);
      C(i+3,j) += A(i+3,k+1)*B(k+1,j);
      C(i+3,j) += A(i+3,k+2)*B(k+2,j);
      C(i+3,j) += A(i+3,k+3)*B(k+3,j);
      //    }
  }
}

void superfast_matmul(double *A, double *B, double *C)
{  
  for (int i = 0; i < N; i += BLOCK) {  // advance rows of C.
    for (int k = 0; k < N; k += BLOCK) { // advance rows of B.
      //macro_kernel_1x4(&A[i*N], B, &C[i*N], i, k);
      macro_kernel_4x4(A, B, C, i, k);
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
