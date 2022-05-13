// extern "C" {
// #include "a.h"
// }
#include "a1.hpp"
#include <cstdlib>

void bar(int i) {
  AWESOME_int j = i;
  foo(j);
}

int main(int argc, char* argv[]) {
  int i = atoi(argv[1]);
  bar(i);

  return 0;
}
