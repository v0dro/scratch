// Solve Ex. 3.5.1.1 using various approaches and study why it works the way it
// does. Problem is solved using a (slow) complete search and (fast) dynamic programming.

#include <iostream>
#include <vector>
using namespace std;

#define SMALL -9999

// 20 3
// 3
// 6 4 8
// 2 
// 10 6
// 4
// 7 3 1 5

// 12 3
// 3
// 6 4 8
// 2 
// 5 10
// 4
// 1 5 3 5

int budget,  garments;
vector<vector<int> > g;
vector<vector<int> > memo(210, vector<int>(20,-1));

// TODO: Get below the abstraction and understand why exactly this works. Step by stpe.
int complete_search(int money, int g_num) {
  if (money < 0) return SMALL;
  if (g_num == garments) return budget - money;
  int ans = -1;
  for (int j = 0; j < g[g_num].size(); ++j) {
    ans =  max(complete_search(money - g[g_num][j], g_num+1), ans);
  }
  return ans;
}

int dynamic_prog(int money, int g_num) {
  if (money < 0) return SMALL;
  if (g_num == garments) return budget - money;
  int ans = -1;
  if (memo[money][g_num] != -1)
    return memo[money][g_num];
  for (int j = 0; j < g[g_num].size(); ++j) {
    ans =  max(complete_search(money - g[g_num][j], g_num+1), ans);
  }
  memo[money][g_num] = ans;
  
  return ans;  
}

int main(int argc, char *argv[])
{
  cin >> budget >> garments;
  g.resize(garments);
  for (int i = 0; i < garments; ++i) {
    int k;
    cin >> k;
    g[i].resize(k);
    
    for (int j = 0; j < k; ++j) {
      int price;
      cin >> price;
      g[i][j] = price;
    }
  }

  cout << "max spend (c): " << complete_search(budget, 0) << endl;
  cout << "max spend (d): " << dynamic_prog(budget, 0) << endl;
  
  return 0;
}
