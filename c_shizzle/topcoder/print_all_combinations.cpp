// get a list of numbers and print all possible combinations of them.

#include <iostream>
#include <vector>
using namespace std;

void combine(vector<int> arr) {
  
}


int main(int argc, char *argv[])
{
  int n;
  vector<int> sums;

  cin >> n;
  
  for (int i = 0; i < n; ++i) {
    int temp;
    cin >> temp;
    sums[i] = temp;
  }

  combine(sums);
  
  return 0;
}
