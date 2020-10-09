#include <iostream>
#include <vector>
using namespace std;

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

  while (true) {
    for (int i = 0; i < n; ++i) {

      if (a[i] % 2 == 0 && done_div == false) {
        a[i] = a[i] / 2;
        done_div = true;
      }
      else {
        a[i] = a[i] * 3;
      }
    }

    if (done_div == true) {
      ops += 1;
      done_div = false;
    }
    else {
      break;
    }
  }

  cout << ops << endl;


  return 0;
}
