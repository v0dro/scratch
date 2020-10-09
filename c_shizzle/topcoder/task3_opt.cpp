#include <iostream>
#include <vector>
using namespace std;

int f(long long int x) {
  if (x % 2 != 0) {             // odd number
    return 0;
  }
  return f(x/2) + 1;
}

int main(int argc, char *argv[])
{
  int n;
  std::vector<long long int> a;
  int ops = 0;

  cin >> n;
  a.resize(n);
  for (int i = 0; i < n; ++i) {
    int temp;
    cin >> temp;
    a[i] = temp;
  }
  bool done_div = false;

  for (int i = 0; i < n; ++i) {
    ops += f(a[i]);
  }

  cout << ops << endl;


  return 0;
}
