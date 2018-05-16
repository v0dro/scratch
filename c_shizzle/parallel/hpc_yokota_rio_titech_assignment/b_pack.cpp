#include <iostream>
using namespace std;

#define NR 5
#define KC 5
#define MR 5
#define NC 10
#define MC 10

#define N 20

int main()
{
  int b[N*N], a[N*N];
  int packb[KC*MC];
  int packa[NC*KC];

  for (int i=0; i<N*N; ++i) {
    b[i] = i+1;
    a[i] = i+1;
  }

  cout << "b: " << endl;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << b[i*N + j] << " ";
    }
    cout << endl;
  }

  // cout << "a: " << endl;
  // for (int i = 0; i < N; ++i) {
  //   for (int j = 0; j < N; ++j) {
  //     cout << a[i*N + j] << " ";
  //   }
  //   cout << endl;
  // }

  int *b_ptr;
  int *temp, *t;

  temp = packb;
  for (int i = 0; i < MC; i += NR) {
    for (int j = 0; j < KC; j += 1) {
      b_ptr = &b[j*N + i];
      for (int k = 0; k < NR; ++k) {
        *(temp++) = *(b_ptr++);
      }
    }
  }

  // temp = packa;
  // int *a_ptr = a;
  // for (int i = 0; i < NC; ++i) {
  //   a_ptr = &a[i*N];
  //   for (int j = 0; j < KC; ++j) {
  //     *(temp) = *(a_ptr);
  //     temp++; a_ptr++;
  //   }
  // }

  // cout << "packa\n";
  // for(int i = 0; i < MR; ++i) {
  //   for (int j = 0; j < KC; ++j) {
  //     cout << packa[i*KC + j] << " ";
  //   }
  //   cout << endl;
  // }

  cout << "packb\n";
  for (int i = 0; i < KC; ++i) {
    for (int j = 0; j < MC; ++j) {
      cout << packb[i*MC + j] << " ";
    }
    cout << endl;
  }

  for (int i = 0; i < KC*MC; ++i) {
    cout << packb[i] << " ";
  }
}
