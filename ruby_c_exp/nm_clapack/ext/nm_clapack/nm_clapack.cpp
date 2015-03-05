#include <ruby.h>
#include <iostream>
#include "f2c.h"
#include "_clapack.h"
#include "nmatrix.h"
using namespace std;

void dgesv(VALUE coeffs, VALUE rhs)
{
  doublereal *coeff_elements = 
    reinterpret_cast<doublereal*>(NM_DENSE_ELEMENTS(coeffs));
  doublereal *rhs_elements = 
    reinterpret_cast<doublereal*>(NM_DENSE_ELEMENTS(rhs));
  integer N = NM_SHAPE0(coeffs);
  integer nrhs = 1;
  integer lda = NM_SHAPE0(coeffs);
  integer ipiv[3];
  integer ldb = NM_SHAPE1(coeffs);
  integer info;

  dgesv_(&N, &nrhs, coeff_elements, &lda, ipiv, rhs_elements, &ldb, &info);
}

void sgesv(VALUE coeffs, VALUE rhs)
{

  real* coeff_elements = (real*)(NM_STORAGE_DENSE(coeffs)->elements);
  real* rhs_elements = (real*)(NM_STORAGE_DENSE(rhs)->elements);

  integer N    = 3;
  integer nrhs = 1;
  integer lda  = 3;
  integer ldb  = 3;
  integer ipiv[3];
  integer info;

  sgesv_(&N, &nrhs, coeff_elements, &lda, ipiv, rhs_elements, &ldb, &info);
}

VALUE call_gesv(VALUE coeffs, VALUE rhs)
{  
  switch(NM_DTYPE(coeffs)) {
    case 5 :
      sgesv(coeffs, rhs);
      break;
    case 6 :
      dgesv(coeffs, rhs); 
      break;
    default :
      rb_raise(rb_eTypeError, "invalid data type");
  }
  return rhs;
}

extern "C" {

  static VALUE solve_gesv(VALUE self, VALUE coeffs, VALUE rhs)
  { 
    return call_gesv(coeffs, rhs);
  }

  void Init_nm_clapack()
  {
    VALUE cNMatrix = rb_define_class("NMatrix",rb_cObject);
    VALUE mCLAPACK = rb_define_module_under(cNMatrix, "CLAPACK");
    rb_define_singleton_method(mCLAPACK, "__solve_gesv__", (METHOD)solve_gesv, 2);
  }
}