#include <ruby.h>
// #include "nmatrix.h"

// template <typename T>
// void cpp_creator(VALUE nmatrix)
// {
//   T* a = (T*)NM_DENSE_ELEMENTS(nmatrix);

//   printf("%lf\n", a[0]);
//   printf("%lf\n", a[1]);
//   printf("%lf\n", a[2]);
//   printf("%lf\n", a[3]);
// }

// void caller_cpp_creator(VALUE nmatrix)
// { 
//   if (NM_DTYPE(nmatrix) == 5) {
//     cpp_creator<float>(nmatrix);
//   }
//   else if (NM_DTYPE(nmatrix) == 6) {
//     cpp_creator<double>(nmatrix);
//   }
// }

extern "C" {
  static VALUE hello_world(VALUE mod)
  {
    return rb_str_new2("hello world!");
  }

  static VALUE __creator__(VALUE self, VALUE nmatrix) 
  { 
    long long int i;
    double mult=0;
    RARRAY_PTR(nmatrix);
    for (i = 0; i < 500000; ++i)
    {
      mult += 4*4;
    }

    printf("%lf\n", mult);
    // caller_cpp_creator(nmatrix);

    // printf("%d\n", NM_DTYPE(nmatrix));
    // return nmatrix;
  }

  void Init_ctest()
  {
    VALUE mCTest = rb_define_module("CTest");
    rb_define_singleton_method(mCTest, "hello_world", (METHOD)hello_world, 0);
    rb_define_singleton_method(mCTest, "__creator__", (METHOD)__creator__, 1);
  }
} // end extern C