#include <ruby.h>

static VALUE hello_world(VALUE mod)
{
  return rb_str_new2("hello world!");
}

void Init_ctest()
{
  VALUE mCTest = rb_define_module("CTest");
  rb_define_singleton_method(mCTest, "hello_world", hello_world, 0);
}