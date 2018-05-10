#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <xmmintrin.h>
using namespace std;

// nrows and ncols
int M, N;
int lda, ldb, ldc;

// numrows of the matrix block
#define NC 10 // 10

// individual register block sizes
#define MR 5 // 5
#define NR 5 // 5

// cache block sizes
#define MC 10 // 10
#define KC 10 // 10

// memalign parameter
#define GEMM_SIMD_ALIGN_SIZE 32

typedef struct aux_t {
  int nc, mc, kc, nr, mr;
} aux_t;

double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return double(tv.tv_sec)+double(tv.tv_usec)*1e-6;
}

double *malloc_aligned(int m, int n, int size)
{
  double *ptr;
  int    err;
  err = posix_memalign( (void**)&ptr, (size_t)GEMM_SIMD_ALIGN_SIZE, size * m * n );

  if ( err ) {
    cout << "bl_malloc_aligned(): posix_memalign() failures";
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
      cout << a[i*ncols + j] << " ";
    }
    cout << endl;
  }
}

void print_arr(double *a, int size, char *desc)
{
  printf("%s\n", desc);
  for (int i = 0; i < size; ++i) {
    cout << a[i] << " ";
  }
  cout << endl;
}
