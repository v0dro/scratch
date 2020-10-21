#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h> 
using namespace std;

int main(int argc, char *argv[])
{
  int N, M;
  vector<vector<long long int> > list;
  vector<vector<long long int> > sums;

  cin >> N >> M;

  for (int i = 0; i < N; ++i) {
    vector<long long int> scores(3);
    int x, y, z;
    cin >> x >> y >> z;
    scores[0] = x; scores[1] = y; scores[2] = z;
    list.push_back(scores);
  }

  long long int max = -1E18;
  
  for (int i = 0; i < N; ++i) {
    vector<long long int> sum(2); // sum, index
    for (int j = 0; j < M; ++j) {
      
    }

  }

  return 0;
}
