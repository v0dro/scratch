#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <immintrin.h>
#include <cmath>
#include <errno.h>

// // caches for Xeon E5-2637 v4 - Intel. TSUBAME login node.
// #define L1 128*1024/4/8 //= 4096 doubles => size * bytes / cores / sizeof(double)
// #define L2 1024*1024/4/8 // = 32,768
// #define L3 10*1024*1024/4/8 // = 3,27,680

// caches for Xeon Intel(R) Xeon(R) CPU E5-2680 v4 @ 2.40GHz. TSUBAME f-node.
// cache values are per-core.
#define L1 32*1024/8 // size in kb * bytes per kb / sizeof(double)
#define L2 256*1024/8
#define L3 35*1024*1024/8 // L3 cache is per chip so shared among cores.
// nrows and ncols
int M, N;
int lda, ldb, ldc;

// individual register block sizes
#define NR 8 // ncols of B micro-panel
#define MR 8 // nrows of A micro-panel

#define KC L1/NR // nrows of B panel / ncols of A panel
// numrows of the matrix block
#define NC L3/KC // 10
// cache block sizes
#define MC 8*NR // width of B panel. resides in L2.

// NR * KC -> L1
// MC * KC -> L2
// KC * NC -> L3

#define MR_INCR 4

// memalign parameter
#define GEMM_SIMD_ALIGN_SIZE 32

typedef struct aux_t {
  int nc, mc, kc, nr, mr, kc_min, mc_min;
} aux_t;

void superfast_matmul(double *A, double *B, double *C, aux_t *aux);
  
double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return double(tv.tv_sec)+double(tv.tv_usec)*1e-6;
}

double *malloc_aligned(int m, int n, int size)
{
  double *ptr;

  ptr = (double*)aligned_alloc((size_t) GEMM_SIMD_ALIGN_SIZE, m*n*size);
  if ( ptr == NULL ) {
    std::cout << "malloc_aligned failed. failure reason : ";
    if (errno == ENOMEM) {
      std::cout << "insufficient memory.";
    }
    else if (errno == EINVAL) {
      std::cout << "alignment not a power of two.";
    }

    exit( 1 );    
  }    
  return ptr;
}

void reset_matrix(double* C, int N, double val)
{
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      C[i*N + j] = val;
    }
  }
}

void print_mat(double *a, int nrows, int ncols, char* desc)
{
  printf("%s\n", desc);
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      std::cout << a[i*ncols + j] << " ";
    }
    std::cout << std::endl;
  }
}

void print_arr(double *a, int size, char *desc)
{
  printf("%s\n", desc);
  for (int i = 0; i < size; ++i) {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;
}

void generate_data(double *A, double* B, double *C, int N)
{
  #pragma omp parallel for
  for (int i=0; i < N; ++i) {
    for (int j=0; j < N; ++j) {
      A[i*N + j] = i*N + j;
      B[i*N + j] = i*N + j;
      C[i*N + j] = 0;
    }
  }
}

// stable : https://github.com/v0dro/scratch/blob/705edb56975615cb5311ab2ff791f2e0f2434843/c_shizzle/parallel/hpc_yokota_rio_titech_assignment/main.cpp
