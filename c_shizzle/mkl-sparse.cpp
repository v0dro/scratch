#include <mkl.h>
#include <mkl_spblas.h>
#include <iostream>

int main(int argc, char *argv[])
{
  int nrows = 5, ncols = 5;
  int pointers[6];
  int indices[13]   = {0,  1,  3,  0, 1, 2, 3, 4,  0, 2, 3, 1,  4};
  double values[13] = {1, -1, -3, -2, 5, 4, 6, 4, -4, 2, 7, 8, -5};
  double x[nrows] = {1, 1, 1, 1, 1};
  double b[nrows];

  pointers[0] = 0;
  pointers[1] = 3;
  pointers[2] = 5;
  pointers[3] = 8;
  pointers[4] = 11;
  pointers[5] = 13;
  
  sparse_matrix_t A;
  matrix_descr desc;

  desc.type = SPARSE_MATRIX_TYPE_GENERAL;
  mkl_sparse_d_create_csr(&A, SPARSE_INDEX_BASE_ZERO, nrows, ncols, pointers, pointers+1, indices, values);
  mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, 1.0, A, desc, SPARSE_LAYOUT_ROW_MAJOR, x, 1, 1, 0.0, b, 1);

  for (int i = 0; i < nrows; ++i) {
    std::cout << b[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
