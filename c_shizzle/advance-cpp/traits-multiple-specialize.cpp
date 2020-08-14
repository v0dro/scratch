#include <iostream>

enum class OP_ENUM {ADD_OP, SUB_OP};

using add_op = std::integral_constant<int, 0>;
using sub_op = std::integral_constant<int, 1>;

template <typename operation, size_t n>
struct AtomicAdd;


template <>
struct AtomicAdd<add_op, 1> {
  template <typename T>
  inline void operator() (T *self, T *src) {
    *self += *src + 100;
  }
};

template <>
struct AtomicAdd<add_op, 5> {
  template <typename T>
  inline void operator() (T *self, T *src) {
    *self += *src + 500;
  }

  inline void operator() (int *self, int *src) {
    *self += *src + 1000;
  }
};

int main(int argc, char *argv[])
{
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);

  AtomicAdd<add_op, 1>()(&a, &b);
  std::cout << "a1: " << a << std::endl;
  AtomicAdd<add_op, 5>()(&a, &b);
  std::cout << "a2: " << a << std::endl;
  return 0;
}
