// Very simple demo of type traits using integral_constant.

#include <iostream>
// #include <type_traits>

// template <typename scalar_t>
// void foo(scalar_t bar) {
//   std::cout << "other: " << bar << std::endl;
// }

// void foo(bool bar) {
//   std::cout << "bool: " << bar << std::endl;
// }

enum class OP_ENUM {ADD_OP, SUB_OP, MUL_OP, DIV_OP};

using add_op = std::integral_constant<int, 0>;
using sub_op = std::integral_constant<int, 1>;

template <typename operation>
struct func {
  template <typename T>
  inline void operator() (T *self, T * src) {
    return;
  }  
};

template <>
struct func<add_op> {
  template <typename T>
  inline void operator() (T *self, T * src) {

    *self += *src;
  
  }
};

template <>
struct func<sub_op> {
  template <typename T>
  void operator() (T *self, T * src) {
    *self -= *src;
  }
};


int main(int argc, char *argv[])
{
  // foo(true);
  // foo(4);

  double a = atoi(argv[1]);
  double b = atoi(argv[2]);
  
  asm volatile ("# add_op begin");
  func<add_op>()(&a, &b);
  asm volatile ("# add_op end");
    
  std::cout << "a: " << a << std::endl;
  
  asm volatile ("# sub_op begin");
  func<sub_op>()(&a, &b);
  asm volatile ("# sub_op begin");
  
  std::cout << "a: " << a << std::endl;
  return 0;
}
