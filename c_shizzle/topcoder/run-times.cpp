#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

// hackerrank competitions have an uppler limit of 2 sec for execution time.

int main() {
  int max = 1e5;
  int qq= 0;
  vector<int> arr(max);

  // time: 7378
  auto start = std::chrono::system_clock::now();
  for (int i = 0; i < max; ++i) {
    for (int j = 0; j < max; ++j) {
      arr[i] = arr[j] + arr[i] * 2;
    }
  }
  auto stop = std::chrono::system_clock::now();
  auto time_s = chrono::duration_cast<chrono::milliseconds>(stop-start).count();
  
  cout << "time: " << time_s << endl;

  // time: 3699
  start = std::chrono::system_clock::now();
  for (int i = 0; i < max; ++i) {
    for (int j = i+1; j < max; ++j) {
      arr[i] = arr[j] + arr[i] * 2;
    }
  }
  stop = std::chrono::system_clock::now();
  time_s = chrono::duration_cast<chrono::milliseconds>(stop-start).count();

  cout << "time: " << time_s << endl;
}
