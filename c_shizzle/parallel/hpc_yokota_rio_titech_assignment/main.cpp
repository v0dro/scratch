// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include "config.hpp"

#define A(i,j) A[(i)*N + j]
#define B(i,j) B[(i)*N + j]
#define C(i,j) C[(i)*N + j]
#define BLOCK 4

void micro_kernel(double *A, double *B, double *C, int mr, int kc, int nr);

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
      // 0th row
      C(0,j) += A(0,k)  *B(k,j);
      C(0,j) += A(0,k+1)*B(k+1,j);
      C(0,j) += A(0,k+2)*B(k+2,j);
      C(0,j) += A(0,k+3)*B(k+3,j);

      // 1st row
      C(1,j) += A(1,k)  *B(k,j);
      C(1,j) += A(1,k+1)*B(k+1,j);
      C(1,j) += A(1,k+2)*B(k+2,j);
      C(1,j) += A(1,k+3)*B(k+3,j);

      // 2nd row
      C(2,j) += A(2,k)  *B(k,j);
      C(2,j) += A(2,k+1)*B(k+1,j);
      C(2,j) += A(2,k+2)*B(k+2,j);
      C(2,j) += A(2,k+3)*B(k+3,j);

      // 3rd row
      C(3,j) += A(3,k)  *B(k,j);
      C(3,j) += A(3,k+1)*B(k+1,j);
      C(3,j) += A(3,k+2)*B(k+2,j);
      C(3,j) += A(3,k+3)*B(k+3,j);
  }
}

void macro_kernel(double *A, double *B, double *C, int nc, int kc, int mc)
{
  // inside this loop is a block of A of dim MC x KC and B of dim KC x NC.
  // now we need to iterate over the elements of these make it run.

  // the block of B is vertically divided into blocks of width (cols) NR, and the
  // block of A into blocks of length MR. The multiplication between these
  // blocks needs to be treated like a matrix multiplication by itself and
  // should therefore consume its own triple nested loop.

  // Imagine it this way: the nc block is the constraint of the matrix that
  // you need to work under. The topmost NC loop only iterates over that
  // much loop space. All the loops below need to do the full work of
  // multiplying the full matrix that falls under NC.

  // The names given to the blocks are very indicative. The m,n,k denote dims
  // of the main matrices. The mc,nc and kc denote the dims of the inner blocks
  // as made by the immediately inner loops and the mr, nr and kr denote the
  // innermost blocks where the actual matrix multiplication happens.

  // Now, since we make a jump of NC in the outermost loop, we need to compute
  // the result for every single row of C, just like we computed the result
  // for each row of C individually when the NC-like outermost loop jump
  // was only 4.

  // Below loops loop over the blocks of the inner blocks of B with dim KC x NR.
  // Treat this like the blocked matrix multiplication in the previous loops. These
  // also need to multiply the specific elements within them and compute a point-by-point
  // matrix product.
  // for (int mr = mc; mr < mc + MC; mr += MR) {
  //   for (int nr = nc; nr < nc + NC; nr += NR) {
  //     micro_kernel(A, B, C, mr, nr, kc);
  //   }
  // }

  // for (int i = nc; i < nc + NC; ++i) {
  //   for (int k = kc; k < kc + KC; ++k) {
  //     for (int j = mc; j < mc + MC; ++j) {
  //       micro_kernel(A, B, C,)
  //       //C(i,j) += A(i,k)*B(k,j);
  //     }
  //   }
  // }

  for (int mr = nc; mr < nc + NC; mr += MR) {
    for (int k = kc; k < kc + KC; k += KC) {
      for (int nr = mc; nr < mc + MC; nr += NR) {
        micro_kernel(A, B, C, mr, k, nr);
        //C(i,j) += A(i,k)*B(k,j);
      }
    }
  }       
}

// multiply micro-panels of size MR x KC and KC x NR.
void micro_kernel(double *A, double *B, double *C, int mr, int kc, int nr)
{
  for (int i = mr; i < mr + MR; ++i) {
    for (int k = kc; k < kc + KC; ++k) {
      for (int j = nr; j < nr + NR; ++j) {
        C(i,j) += A(i,k)*B(k,j);
      }
    }
  }
}

void superfast_matmul(double *A, double *B, double *C)
{  
  for (int nc = 0; nc < N; nc += NC) {  // advance rows of C and A. like i.
    for (int kc = 0; kc < N; kc += KC) { // advance rows of B and cols of A. like k.
      for (int mc = 0; mc < N; mc += MC) { // like j.
        macro_kernel(A, B, C, nc, kc, mc);
      }
        //      macro_kernel_4x4(&A[i*N], B, &C[i*N], i, k);
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "provide nrows and ncols" << endl;
    exit(1);
  }

  if (MC < MR) {
    cout << "wrong config: MC should be greater than MR." << endl;
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
