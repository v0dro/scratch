#include <iostream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

// width of the matrix block ()
#define NC 1

// individual register block sizes
#define MR 1
#define NR 1

// cache block sizes
#define MC 5
#define KC 5

// nrows and ncols
int M, N;

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
