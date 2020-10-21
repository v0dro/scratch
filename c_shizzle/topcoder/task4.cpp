#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h> 
using namespace std;

int main(int argc, char *argv[])
{
  int N, M;
  vector<vector<long long int> > list;
  vector<long long int> sums;

  cin >> N >> M;

  for (int i = 0; i < N; ++i) {
    vector<long long int> scores(3);
    int x, y, z;
    cin >> x >> y >> z;
    scores[0] = x; scores[1] = y; scores[2] = z;
    list.push_back(scores);
  }

  // for M=1
  long long int sum = fabs(list[0][0]) + fabs(list[0][1]) + fabs(list[0][2]);
  long long int max = sum;
  int cntr;


  // This solution is wrong because it assumes that the sum of absolute numbers
  // is the same as absolute of the sum.
  
  // It is merely taking abs(val) for each number, summing it and appending it
  // to the list. Then taking the top numbers and showing their sum.

  // The approach would be add M numbers of each column, get their abs and then
  // add this absolute. This can then be done for every set and then we can only
  // report the largest of these numbers as the result.
  for (int i = 0; i < N; ++i) {
    if (sum > max) {
      max = sum;
      cntr = i;
    }
    sum = fabs(list[i][0]) + fabs(list[i][1]) + fabs(list[i][2]);
    sums.push_back(sum);
  }

  sort(sums.begin(), sums.end());

  long long int s = 0;
  for (int i = M-1; i < N; ++i) {
    s += sums[i];
  }


  cout << s << endl;

  return 0;
}
