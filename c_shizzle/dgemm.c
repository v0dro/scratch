
#include <stdio.h>
#include <stdlib.h>
#include <lapacke.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* void dgetrf_(int* M, int* N, double* A, int* LDA, int* IPIV, int* INFO); */
/* void dtrsm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, double* ALPHA, double* A, int* LDA, double* B, int* LDB); */

/* void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K, double* ALPHA, double* A, int* LDA, double* B, int* LDB, double* BETA, double* C, int* LDC); */


/* void dgemv_(char* TRANS, int* M, int* N, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY); */

/* void dgemm() */
/* { */
/*   int nrows = 3; */
/*   int ncols = 3; */
/*   double a[9] = {1,2,3,4,5,6,7,8,9}; */
/*   double b[9] = {1,2,3,4,5,6,7,8,9}; */
/*   double *c = (double*)malloc(sizeof(double)*nrows*ncols); */
/*   char c_n = 'n'; */
/*   double m1 = 1; double p1 = 1; */
/*   int i; */
  
/*   dgemm_(&c_n, &c_n, &nrows, &ncols, &nrows, &m1, a, &nrows, b, &ncols, &p1, c, &nrows); */

/*   for (i = 0; i < nrows*ncols; ++i) { */
/*     printf(" %f ", c[i]); */
/*   } */
/* } */

// https://stackoverflow.com/a/21974753
// http://www.netlib.no/netlib/lapack/double/dgeqrf.f
void qr()
{
  // calculate tau and Q
  int nrows = 3, i;
  int ncols = 3;
  double a[9] = {4,4,1,8,10,1,3,54,32};
  double *tau = calloc(ncols,sizeof(double));
  
  LAPACKE_dgeqrf(LAPACK_ROW_MAJOR, nrows, ncols, a, nrows, tau);

  printf("tau:\n\n");
  for (i = 0; i < nrows; ++i) {
    printf(" %f ", tau[i]);
  }

  // use results of tau and Q to calculate QR.
  char side = 'L';
  char trans = 'N';
  double *c = (double*)calloc(nrows*ncols,sizeof(double));
  for (int i = 0;i < nrows; ++i) c[i*nrows + i] = 1.0;
  int k = 1;

  LAPACKE_dormqr(LAPACK_ROW_MAJOR, side, trans, nrows, ncols, k, a, nrows, tau, c, nrows);

  for (i = 0; i < nrows*ncols; ++i) {
    printf(" %f ", c[i]);
  }
}

int main()
{
  //dgemm();  gcc dgemm.c -L/usr/lib/libblas/ -L/usr/lib/ -lblas -llapacke 
  //  problem happens if you install lapacke from ubuntu sources since the latest version
  // depens on a strage version of blas.
  // https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=810230
  qr();
}
