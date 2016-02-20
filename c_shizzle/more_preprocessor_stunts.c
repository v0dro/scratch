#include <stdio.h>

#define NM_DEF_ENUM(name, ...)     \
  typedef enum nm_ ## name {       \
    __VA_ARGS__                    \
  } nm_ ## name;

NM_DEF_ENUM(dtype_t,    BYTE                =  0,  // unsigned char
                        INT8                =  1,  // char
                        INT16               =  2,  // short
                        INT32               =  3,  // int
                        INT64               =  4,  // long
                        FLOAT32         =  5,  // float
                        FLOAT64         =  6,  // double
                        COMPLEX64       =  7,  // Complex64 class
                        COMPLEX128  =  8,  // Complex128 class
                        RUBYOBJ         = 9);  // Ruby VALUE type

int main()
{ 
  nm_dtype_t e;

  e = BYTE;
  printf("BYTE : %d\n", e);
  return 0;
}