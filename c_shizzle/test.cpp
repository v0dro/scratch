#include <iostream>
using namespace std;

int main()
{
  int **m;

  m = new int*[5];
  for (int i = 0; i < 5; ++i) {
    m[i] = new int[5];
  }

  // for (int i = 0; i < 5; ++i) {
  //   for (int j = 0; j < 5; ++j) {
  //     m[i][j] = 666;
  //   }
  // }

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      cout << m[i][j] << " ";
    }
    cout << "\n";
  }
}