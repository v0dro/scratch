// Simple program for reading numbers from a binary file containing 8*8 integers.

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
  int n = 8;  // length of square matrix
  ifstream file("data.dat", ios::in | ios::binary);
  int data[n*n];

  // read data number-by-number
  for (int i = 0; i < n*n; ++i) {
    file.read((char*)&data[i], sizeof(int));
  }

  // print data
  for (int i = 0; i < n*n; ++i){
    cout << data[i] << " ";
  }
}
