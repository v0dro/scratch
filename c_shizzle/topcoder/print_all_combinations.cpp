// get a list of numbers and print all possible combinations of them.

// The puppose of the stack over here is not for storing the numbers, but
// for keeping a track of  the INDICES that have already been read and put
// into the stack. Think about it terms of the indices that need to be stored
// not the data that needs to be consumed.

#include <iostream>
#include <vector>
using namespace std;

void combine(vector<int> list, int k, int n) {
  vector<int> stack;
  vector<int> result(k);
  int index, val;

  stack.push_back(0);
  while (stack.size() > 0) {
    index = stack.size() - 1;
    val = stack.back();
    stack.pop_back();

    while (val < n) {
      result[index] = list[val];
      index += 1; val += 1;
      stack.push_back(val);
      if (index == k) {
        // print the sequence that has been created
        for (int p = 0; p < k; ++p) {
          cout << result[p] << " ";
        }
        cout << endl;
        break;
      }
    }
  }
}


int main(int argc, char *argv[])
{
  int n, k = 3;
  vector<int> list;

  cin >> n;
  list.resize(n);
  
  for (int i = 0; i < n; ++i) {
    int temp;
    cin >> temp;
    list[i] = temp;
  }

  combine(list, k, n);
  
  return 0;
}
