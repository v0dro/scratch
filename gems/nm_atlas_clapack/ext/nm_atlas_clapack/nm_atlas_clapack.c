#include <ruby.h>

int
calc_factorial(int n)
{
  return (n > 1 ? n*calc_factorial(n-1) : 1);
}

static VALUE
cfactorial(VALUE self, VALUE n)
{
  return INT2NUM(calc_factorial(NUM2INT(n)));
}

void Init_nm_atlas_clapack()
{
  VALUE cFact = rb_define_class("Fact", rb_cObject);
  rb_define_method(cFact, "factorial", cfactorial, 1);
}