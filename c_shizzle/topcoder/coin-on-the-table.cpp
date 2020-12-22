#include <iostream>
#include <vector>
using namespace std;

// https://www.hackerrank.com/challenges/coin-on-the-table/problem?h_r=internal-search&h_r=next-challenge&h_v=zen

// conduct a BFS to figure out the shortest path.
void shortest_path(int start) {
}
  

void search(int m, int n, int k, vector<vector<char>> grid) {
  int i = 1, j = 1;
}

int main(int argc, char *argv[])
{
  int m, n, k;
  vector<vector<char>> grid;
  
  cin >> m >> n >> k;
  grid.resize(m);
  for (int i = 0; i < m; i++) {
    grid[i].resize(n);
    char temp;
    for (int j = 0; j < n; j++) {
      cin >> temp;
      grid[i][j] = temp;
    }
  }
  
  search(m, n, k, grid);
  return 0;
}
