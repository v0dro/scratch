#ifndef SYNC_LU_DECOMP_H
#define SYNC_LU_DECOMP_H

#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include "implementation.hpp"
#include "unit_tests.hpp"
using namespace std;

void print_block(double *a, int nrows, int ncols, int proc_id)
{
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      cout << "(" << proc_id << "," << i << "," << j << "," << a[i*nrows + j] << ") ";
    }
    cout << endl;
  }
}

#endif /* SYNC_LU_DECOMP_H */
