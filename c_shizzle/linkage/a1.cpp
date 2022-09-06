#include <iostream>

#include "a1.hpp"

void foo(AWESOME_int i, void *ptr) {
  std::cout << "this is foo: " << i << std::endl;
  double *d_ptr = (double*)ptr;
  for (AWESOME_int j = 0; j < i; ++j) {
    std::cout << d_ptr[j] << " ";
  }
  std::cout << std::endl;
}
