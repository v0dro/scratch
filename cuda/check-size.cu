#include <iostream>
#include <stdio.h>

const int blocksize = 1;

typedef unsigned char uint40[5];

struct alignas(2) BFloat16 {
  uint16_t x;
  BFloat16() = default;

  // constexpr C10_HOST_DEVICE BFloat16(unsigned short bits, from_bits_t) : x(bits){};
};

__global__
void check_size() {
  uint40 a;
  BFloat16 b16;
  
  printf("sizeof(unsigned int): %d\n", sizeof(unsigned int));
  printf("sizeof(char): %d\n", sizeof(char));
  printf("sizeof(size_t): %d\n", sizeof(size_t));
  printf("sizeof(unsigned long long int): %d\n", sizeof(unsigned long long int));
  printf("sizeof(double): %d\n", sizeof(double));
  printf("sizeof(uint40 a): %d\n", sizeof(a));
  printf("sizeof(int): %d\n", sizeof(int));
  printf("sizeof(unsigned short int): %d\n", sizeof(unsigned short int));

  printf("\n---------------------\n");
  printf("sizeof(unsigned int*): %d\n", sizeof(unsigned int*));
  printf("sizeof(char*): %d\n", sizeof(char*));
  printf("sizeof(size_t*): %d\n", sizeof(size_t*));
  printf("sizeof(unsigned long long int*): %d\n", sizeof(unsigned long long int*));
  printf("sizeof(double*): %d\n", sizeof(double*));
  printf("sizeof(uint40*): %d\n", sizeof(uint40*));
  printf("sizeof(BFloat16*): %d\n", sizeof(BFloat16*));
  printf("sizeof(int*): %d\n", sizeof(int*));
  printf("sizeof(unsigned short int*): %d\n", sizeof(unsigned short int*));

  
  printf("\n---------------------\n");
  printf("a: %ld, a+1: %ld\n", &a, &a+1);
  printf("&b16: %ld, &b16+1: %ld\n", &b16, &b16+1);
}

int main(int argc, char *argv[])
{
  dim3 dimBlock(blocksize, 1);
  dim3 dimGrid(1,1);

  check_size<<<dimGrid, dimBlock>>>();
  cudaDeviceSynchronize(); 
  return 0;
}
