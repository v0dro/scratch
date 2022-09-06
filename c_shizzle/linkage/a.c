#include <stdio.h>

#include "a.h"

void foo(AWESOME_int a, void *ptr) {
  printf("this is foo %lld.\n", a);
  double *d_ptr = (double*)ptr;
  for (AWESOME_int j = 0; j < a; ++j) {
    printf("%lf ", d_ptr[j]);
  }
  printf("\n");
}
