#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
  int size = 4, i, j;
  double *a = (double*)malloc(sizeof(double)*size*size);

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      a[i*size + j] = (i + j + 1)*2;
    }
  }

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      cout << a[i*size + j] << " ";
    }
    cout << endl;
  }

  for (int k = 0; k < size; ++k) {
    a[k*size + k] = 1/a[k*size + k]; // get inverse of the diag element.
    // distribute this inverse over the rows and columns.
    for (i = k+1; i < size; ++i) {
      a[k*size + i] = a[k*size + i]*a[k*size + k];
      a[k + i*size] = a[k + i*size]*a[k*size + k];
    }

    // multiply the rows and cols with the lower right corner elements.
    for (i = k+1; i < size; ++i) {
      for (j = k+1; j < size; ++j) {
        a[i*size +j] = a[i*size + j] - a[i*size + k]*a[k*size + j];
      }
    }
  }

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      cout << a[i*size + j] << " ";
    }
    cout << endl;
  }
}
