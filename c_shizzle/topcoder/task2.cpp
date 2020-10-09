#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  long long int d, n;

  cin >> d >> n;

  if (d == 0) {
    if (n == 100) {
      cout << 101;
    }
    else {
      cout << n;
    }
  }
  else if (d == 1) {
    if (n == 100) {
      cout << 101 * 100;
    } else {
      cout << n * 100;
    }
  }
  else if (d == 2) {
    if (n == 100) {
      cout << 101 * 10000;
    }
    else {
      cout << n * 10000;
    }

  }
  return 0;
}
