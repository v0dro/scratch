#include <ruby.h>
#include "nmatrix.h"
#include "clapack.h"

int clapack_sgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS,
                  float *A, const int lda, int *ipiv,
                  float *B, const int ldb);

int clapack_dgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS,
                  double *A, const int lda, int *ipiv,
                  double *B, const int ldb);
/*
  The C code below will be replaced with C++ templates (using template tables probably)
  once the project is actually started. This is for demo purposes only.
*/
void sgesv(VALUE coeffs, VALUE rhs)
{
  float* coeffs_elems = (float*)NM_DENSE_ELEMENTS(coeffs);
  float* rhs_elems    = (float*)NM_DENSE_ELEMENTS(rhs);

  const int n = 3, 
          nrhs = 1, 
          lda  = 3, 
          ldb  = 3;
  int     ipiv[3];

  clapack_sgesv(CblasRowMajor, n, nrhs, coeffs_elems, lda, ipiv, rhs_elems, ldb);
}

void dgesv(VALUE coeffs, VALUE rhs)
{
  double* coeffs_elems = (double*)NM_DENSE_ELEMENTS(coeffs);
  double* rhs_elems    = (double*)NM_DENSE_ELEMENTS(rhs);

  const int n = 3, 
          nrhs = 1, 
          lda  = 3, 
          ldb  = 3;
  int     ipiv[3];

  clapack_dgesv(CblasRowMajor, n, nrhs, coeffs_elems, lda, ipiv, rhs_elems, ldb);
}

VALUE solve_gesv(VALUE self, VALUE coeffs, VALUE rhs)
{
  switch(NM_DTYPE(coeffs))
  {
    case 5:
      sgesv(coeffs, rhs);
      break;
    case 6:
      dgesv(coeffs, rhs);
      break;
  }

  return rhs;
}

void Init_nm_atlas()
{
  VALUE cNMatrix = rb_define_class("NMatrix",rb_cObject);
  VALUE mCLAPACK_ATLAS = rb_define_module_under(cNMatrix, "CLAPACK_ATLAS");
  rb_define_singleton_method(mCLAPACK_ATLAS, "__solve_gesv__", solve_gesv, 2);
}