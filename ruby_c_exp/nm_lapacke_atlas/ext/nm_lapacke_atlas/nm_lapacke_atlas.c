#include <ruby.h>
#include "nmatrix.h"
#include "lapacke.h"

static void dgesv(VALUE coeffs, VALUE rhs)
{
  lapack_int n = 3, 
          nrhs = 1, 
          lda  = 3, 
          ldb  = 1, 
          ipiv[3];

  double *A = (double*)NM_DENSE_ELEMENTS(coeffs);
  double *b = (double*)NM_DENSE_ELEMENTS(rhs);

  LAPACKE_dgesv( LAPACK_ROW_MAJOR, n, nrhs, A, lda, ipiv,
                          b, ldb );
}

static void sgesv(VALUE coeffs, VALUE rhs)
{
  lapack_int n = 3, 
          nrhs = 1, 
          lda  = 3, 
          ldb  = 1, 
          ipiv[3];

  float *A = (float*)NM_DENSE_ELEMENTS(coeffs);
  float *b = (float*)NM_DENSE_ELEMENTS(rhs);

  LAPACKE_sgesv( LAPACK_ROW_MAJOR, n, nrhs, A, lda, ipiv,
                          b, ldb );
}

static VALUE solve_gesv(VALUE self, VALUE coeffs, VALUE rhs)
{
  switch(NM_DTYPE(coeffs)) {
    case 5 :
      sgesv(coeffs, rhs);
      break;
    case 6 :
      dgesv(coeffs, rhs);
      break;
    default :
      printf("Cannot understand dtype %d\n", NM_DTYPE(coeffs));
  }

  return rhs;
}

void Init_nm_lapacke_atlas()
{
  VALUE cNMatrix = rb_define_class("NMatrix",rb_cObject);
  VALUE mLAPACKE_ATLAS = rb_define_module_under(cNMatrix, "LAPACKE_ATLAS");
  rb_define_singleton_method(mLAPACKE_ATLAS, "__solve_gesv__", solve_gesv, 2);
}