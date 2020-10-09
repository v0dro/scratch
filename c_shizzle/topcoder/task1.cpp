#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
  int a, b;

  cin >> a >> b;
  cout << ((a <= 8 && b <= 8) ? "Yay!" : ":(");
  return 0;
}
