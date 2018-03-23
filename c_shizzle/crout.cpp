#include <cstdio>
#include <iostream>
using namespace std;

void crout(double const A[3][3], double L[3][3], double U[3][3], int n) {
  int i, j, k;
  double sum = 0;

  for (i = 0; i < n; i++) {
    U[i][i] = 1;
  }

  for (j = 0; j < n; j++) {
    for (i = j; i < n; i++) {
      sum = 0;
      for (k = 0; k < j; k++) {
        sum = sum + L[i][k] * U[k][j];	
      }
      L[i][j] = A[i][j] - sum;
    }

    for (i = j; i < n; i++) {
      sum = 0;
      for(k = 0; k < j; k++) {
        sum = sum + L[j][k] * U[k][i];
      }
      if (L[j][j] == 0) {
        printf("det(L) close to 0!\n Can't divide by 0...\n");
      }
      U[j][i] = (A[j][i] - sum) / L[j][j];
    }
  }
}

int main()
{
  double a[3][3] = {2,-1,2,-4,6,3,-4,-2,8}, l[3][3] = {0}, u[3][3] = {0};
  crout(a, l, u, 3);

  cout << "L" << endl;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cout << l[i][j] << " ";
    }
    cout << endl;
  }

  cout << "U" << endl;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cout << u[i][j] << " ";
    }
    cout << endl;
  }
}
