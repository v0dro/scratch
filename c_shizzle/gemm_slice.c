#include <cblas.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  double A[100];
  double B[100];

  for (int i = 0; i < 100; ++i) {
    A[i] = 2;
    B[i] = 4;
  }

  printf("Printing A:\n");
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      printf("%0.3lf ", A[i*10 + j]);
    }
    printf("\n");
  }
  printf("Printing B:\n");
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      printf("%0.3lf ", B[i*10 + j]);
    }
    printf("\n");
  }

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 2, 2, 2, 1.0, A + 10*2 + 2,
              10, B + 8, 10, 0, A+10*5+4, 10);

  printf("Printing A:\n");
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      printf("%0.3lf ", A[i*10 + j]);
    }
    printf("\n");
  }
  return 0;
}
