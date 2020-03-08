#include <iostream>
#include <atomic>
#include <string>
#include <functional>
#include <map>
#include <unordered_map>
#include <cstdlib>

#define REDUCE_FUNCTOR(FUNCTOR_NAME, OPERATOR) class FUNCTOR_NAME {     \
public:                                                                 \
 FUNCTOR_NAME() {};                                                     \
 template <typename T>                                                  \
 void operator() (T* self_data, const T* src_data) {                    \
   *self_data OPERATOR *src_data;                                       \
 };                                                                     \
 };                                                                     \
FUNCTOR_NAME FUNCTOR_NAME ## _func;

REDUCE_FUNCTOR(ReduceAdd, +=);

// template <typename T>
// void foo(T& a, T val) {
//   std::atomic<T> bar(a);
//   T old = bar;
//   while (!bar.compare_exchange_weak(old, old +val));
// }

// template <typename T>
// void inplace_add(T& a, const T b) {
//   a += b;
// }

// template <typename T>
// void inplace_subtract(T& a, const T b) {
//   a -= b;
// }

// class AutoAll {
  
// };

// class AutoAdd : public AutoAll
// {
// public:
//   AutoAdd() {
    
//   };
//   void operator() (auto& a, auto& b) { a += b;}
// };

// class AutoSub : public AutoAll
// {
// public:
//   AutoSub() {};
//   void operator() (auto& a, auto& b) { a -= b;}
// };

// enum ENUM_TYPE { C_INT, C_DOUBLE };


// void bar(auto& func_t, auto scalar) {

//   decltype(scalar) baz= 1;
//   func_t(1, 3, baz, 1);
// }

// void foo(ENUM_TYPE e_type, auto num) {
//   switch(e_type) {
//   case C_INT : {
//     using scalar_t = int;
//     using reduce_func = std::function<void(const int&, const int&, scalar_t, const int)>;
//     reduce_func func = [&](const int& idx, const int& i, scalar_t arr, const int scalar) {
//                          arr += scalar;
//                        };
//     bar(func, num);
//     std::cout << "int\n";
//     break;
//   }
//   case C_DOUBLE : {
//     using scalar_t = double;
//     using reduce_func = std::function<void(const int&, const int&, scalar_t, const int)>;
//     reduce_func func = [&](const int& idx, const int& i, scalar_t arr, const int scalar) {
//                          arr -= scalar;
//                        };
//     bar(func, num);
//     std::cout << "double\n";
//     break;
//   }}
// }

// int main(int argc, char *argv[])
// {
//   int stuff = atoi(argv[1]);
//   if (stuff == 0) {
//     foo(C_INT, 2);
  
//   }
//   else if (stuff == 1) {
//     foo(C_DOUBLE, 3.0);
//   }

//   // std::cout << "a: " << a << std::endl;
//   return 0;
// }
