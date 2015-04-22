#include <iostream>
#include <algorithm>
using namespace std;

int main(void)
{
  long int N;
  int score1, score2, lead = 0, leader, player1 = 0, player2 = 0;

  cin >> N;

  for (int i = 0; i < N; ++i) {
    cin >> score1 >> score2;
    
    player1 += score1;
    player2 += score2;

    if (abs(player1 - player2) > lead) {
      lead = abs(player1 - player2);

      if (player1 > player2)
	leader = 1;
      else
	leader = 2;
    }
  }
  
  cout << leader << " " << lead << "\n";

  return 0;
}
