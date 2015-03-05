// #include <stdio.h>
// #include <stdlib.h>

// #define NATIONAL_BITCH \
//   struct bithcy {      \
//     bitch* bill;       \
//   };

// #define WORLD_BITCH \
//   struct bitch {    \
//     NATIONAL_BITCH; \
//   };

// #define HELLO_PRE WORLD_BITCH

// HELLO_PRE
// struct hello : world {
//   int i;
// };
typedef long size_t;
typedef int nm_dtype_t;

#define NM_DECL_ENUM(enum_type, name)   nm_ ## enum_type name

#define NM_DEF_STORAGE_ELEMENTS   \
  NM_DECL_ENUM(dtype_t, dtype);   \
  size_t      dim;                \
  size_t*     shape;              \
  size_t*     offset;             \
  int         count;              \
  STORAGE*   src

#define NM_DEF_STORAGE_STRUCT      \
typedef struct STORAGE {        \
  NM_DEF_STORAGE_ELEMENTS;         \
} STORAGE;

NM_DEF_STORAGE_STRUCT


#define NM_DEF_STRUCT_PRE(name)                typedef struct name {
#define NM_DEF_STRUCT_POST(name)               } name;

NM_DEF_STRUCT_PRE(NM)
NM_DEF_STRUCT_POST(NM)


typedef struct pod {
  struct pod* sec;
} pod;
int main()
{ 
  STORAGE hell;
  // struct bitch bull;
  // bull.bill = 9;
  // void *p;
  // p = (double*)malloc(sizeof(double));

  // *((double*)p) = 33.55;

  // printf("%f\n", *((double*)p));
  // printf("%f\n", (double)p[2]);
  // printf("%c\n", p[0]);
  // printf("%c\n", p[1]);
  // printf("%c\n", p[2]);
  // printf("%c\n", p[3]);
}