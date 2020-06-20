#include <iostream>

template <typename scalar_t>
void foo(scalar_t bar) {
  std::cout << "other: " << bar << std::endl;
}

void foo(bool bar) {
  std::cout << "bool: " << bar << std::endl;
}


int main(int argc, char *argv[])
{
  foo(true);
  foo(4);
  return 0;
}
