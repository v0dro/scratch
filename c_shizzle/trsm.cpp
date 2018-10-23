// simple implementation of trsm in CPP
#include <iostream>
using namespace std;

int main()
{
  double tri[3] = {3136,0.18367,28.2449};
  double A[4] = {16,25,144,169};

  for (int col = 0; col < 2; col++) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < i; j++) {
        cout << "in -> " << " " << A[(i)*2 + col] << " " <<
          A[(j)*2 + col] << " " << tri[(i+1)*i/2 + j] << endl;
        A[(i)*2 + col] -=
          A[(j)*2 + col]*tri[(i+1)*i/2 + j];
      }

      if (tri[(i+1)*i/2 + i] > 10E-10) {
        cout << "diag -> " << A[(i)*2 + i] << " " << tri[(i+1)*i/2 + i] << endl;
        A[(i)*2 + i] /= tri[(i+1)*i/2 + i];
      }
    }
  }

  cout << "A: " << endl;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      cout << A[i*2 + j] << " ";
    }
    cout << endl;
  }
}
