#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <xmmintrin.h>
using namespace std;

// nrows and ncols
int M, N;

// numrows of the matrix block
#define NC 250

// individual register block sizes
#define MR 5
#define NR 5

// cache block sizes
#define MC 10
#define KC 10

// memalign parameter
#define GEMM_SIMD_ALIGN_SIZE 32

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
