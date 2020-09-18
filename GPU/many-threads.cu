#include <stdio.h>
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

const int blocksize = 1024;
const int factor = 16;
const int arraysize = blocksize / factor;

template <typename T>
__global__ void addition_test_kernel(T * a, T * sum) {
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  int idx = (tid) % arraysize;

  sum[idx] += a[idx];

  atomicAdd(&sum[idx], a[idx]);

  if (idx == 2)
    printf("%d %d %d %d -> ", idx, tid, sum[idx], a[idx]);
}

template <typename T>
void test_atomic_int() {
  dim3 dimBlock(blocksize, 1);
  dim3 dimGrid(1, 1);

  T *a, *b, *sum, *answer, *ad, *bd, *sumd, *answerd;
  
  a = (T*)malloc(arraysize * sizeof(T));
  sum = (T*)malloc(arraysize * sizeof(T));
  answer = (T*)malloc(arraysize * sizeof(T));

  for (int i = 0; i < arraysize; ++i) {
    a[i] = 1;
    sum[i] = 0;
    answer[i] = i + i;
  }

  cudaMalloc((void**)&ad, arraysize * sizeof(T));
  gpuErrchk(cudaPeekAtLastError());
  cudaMalloc((void**)&sumd, arraysize * sizeof(T));
  gpuErrchk(cudaPeekAtLastError());
  
  cudaMemcpy(ad, a, arraysize * sizeof(T), cudaMemcpyHostToDevice);
  gpuErrchk(cudaGetLastError());
    
  cudaMemcpy(sumd, sum, arraysize * sizeof(T), cudaMemcpyHostToDevice);
  gpuErrchk(cudaPeekAtLastError());

  printf("addition kernel;\n");
  addition_test_kernel<<<dimGrid, dimBlock>>>(ad, sumd);
  gpuErrchk(cudaPeekAtLastError());
  cudaMemcpy(sum, sumd, arraysize * sizeof(T), cudaMemcpyDeviceToHost);

  cudaDeviceSynchronize();

  for (int i = 0; i < arraysize; ++i) {

    printf("sum[%d]: %d\n", i, sum[i]);    
  }

}

int main(int argc, char *argv[])
{
  test_atomic_int<int32_t>();
  return 0;
}
