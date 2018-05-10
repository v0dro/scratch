// Program for achieving highest flops/sec for matrix multiplication.
// Use SIMD, multi-threading, cache blocking, etc.

#include "config.hpp"

#define A(i,j) A[(i)*N + j]
#define B(i,j) B[(i)*N + j]
#define C(i,j) C[(i)*N + j]
#define BLOCK 4

void packB_KCxMC(double *packB, double *B, int , int , aux_t *);
void packA_NCxKC(double *packA, double *A, int , int , aux_t *);
void micro_kernel(double *A, double *B, double *C, aux_t *);

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

// XA - array of size NCxKC.
// XB - array of size KCxMC.
void macro_kernel(double *XA, double *XB, double *C, int nc_min, int kc_min, int mc_min, aux_t *aux)
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

  // Note that the entire configuration changes when row-major is being done. Algorithms
  // must be entirely reworked for that purpose when changing
  for (int mr = 0; mr < nc_min; mr += MR) {
    for (int nr = 0; nr < mc_min; nr += NR) {
      aux->nr = nr;
      aux->mr = mr;
      micro_kernel(&XA[mr*aux->kc_min], &XB[nr], &C[mr*MR + nr], aux);
    }
  }
}

// multiply micro-panels of size MR x KC and KC x NR.
void micro_kernel(double *A, double *B, double *C, aux_t *aux)
{
  double *A_ptr, * B_ptr, *C_ptr;
  C_ptr = C;
  
  for (int i = 0; i < MR; ++i) {
    A_ptr = &A[i*aux->kc_min];
    for (int k = 0; k < aux->kc_min; ++k) {
      B_ptr = &B[k*aux->mc_min];
      for (int j = 0; j < NR; ++j) {
        *(C_ptr++) += *(A_ptr)*(*B_ptr++);
      }
      A_ptr++;
    }
    C_ptr += ldc;
  }
}

void matmul(double *A, double *B, double *C, aux_t *aux)
{
  double *packA, *packB;
  packA = (double*)malloc(sizeof(double)*NC*KC);
  packB = (double*)malloc(sizeof(double)*KC*MC);
  // each iteration of this loop advances the rows of A and C.
  for (int nc = 0; nc < N; nc += NC) {  // like i.
    aux->nc = nc;
    int nc_min = std::min((N-nc), NC);
    
    // each iteration of this advances the cols of A and rows of B.
    for (int kc = 0; kc < N; kc += KC) { // like k.
      int kc_min = std::min((N-kc), KC);
      aux->kc = kc;
      aux->kc_min = kc_min;
      packA_NCxKC(packA, A, nc_min, kc_min, aux);

      // each iteration advances the cols of B and C.
      for (int mc = 0; mc < N; mc += MC) { // like j.
        aux->mc = mc;
        int mc_min = std::min(N-mc, MC);
        aux->mc_min = mc_min;
        
        packB_KCxMC(packB, B, kc_min, mc_min, aux);
        macro_kernel(packA, packB, &C[nc*nc_min], nc_min, kc_min, mc_min, aux);
      }
    }
  }
}

void packA_NCxKC(double *packA, double *A, int nc_min, int kc_min, aux_t *aux)
{
  double *packA_temp = packA;
  double *A_ptr;
  
  for (int n = aux->nc; n < aux->nc + nc_min; ++n) {
    A_ptr = &A(n,aux->kc);
    for (int k = 0; k < kc_min; ++k) {
      *(packA_temp++) = *(A_ptr++);
    }
  }
}

void packB_KCxMC(double *packB, double *B, int kc_min, int mc_min, aux_t *aux)
{
  double *packB_temp = packB, *temp;
  double *B_ptr;

  for (int k = aux->kc; k < aux->kc + kc_min; k += 1) {
    B_ptr = &B(k, aux->mc);
    for (int m = 0; m < mc_min; m += NR) {
      temp = B_ptr + m;
      for (int i = 0; i < NR; i++) {
        *(packB_temp++) = *(temp++);
      }
    }    
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    std::cout << "provide nrows and ncols" << std::endl;
    exit(1);
  }

  if (MC < MR) {
    std::cout << "wrong config: MC should be greater than MR." << std::endl;
    exit(1);
  }
  
  N = atoi(argv[1]); M = N;

  // if (N % NC != 0){ 
  //   std::cout << "N" << N << "is not a multiple of NC " << NC << endl;;
  //   exit(1);
  // }
  lda = ldb = ldc = N;
  // double A[N*N], B[N*N], C[N*N];
  double *A, *B, *C;
  double start, stop;
  aux_t aux;
  A = (double*)malloc(sizeof(double)*N*N);
  B = (double*)malloc(sizeof(double)*N*N);
  C = (double*)malloc(sizeof(double)*N*N);
  generate_data(A, B, C, N);
  
  start = get_time();
  // print_mat(A, N, N, "A:");
  // print_mat(B, N, N, "A:");
  // print_mat(C, N, N, "A:");
  matmul(A, B, C, &aux);
  stop = get_time();

  std::cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << std::endl;

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

  std::cout << "N = " << N << ". time: " << stop - start << " s. Gflops: " <<
    2.*N*N*N/(stop-start)/1e9 << std::endl;


  double error = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      error += C[i*N + j] - D[i*N + j];
    }
  }

  std::cout << "error: " << error/N/N << std::endl;
  return 0;
}
