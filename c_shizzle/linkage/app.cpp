extern "C" {
#include "a.h"
}
// #include "a1.hpp"
#include <cstdlib>

void bar(int i) {
  AWESOME_int j = i;
  double *arr = (double*)malloc(j * sizeof(double));
  foo(j, arr);
}

int main(int argc, char* argv[]) {
  int i = atoi(argv[1]);
  bar(i);

  return 0;
}
