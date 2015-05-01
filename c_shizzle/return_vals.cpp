#include <iostream>
#include <cstdio>
using namespace std;

int main(void)
{ 
  // ============================================================================
  // check what value cin returns

  int n;

  // cin returns 0 if the value entered in non-numeric, because n is supposed
  // to be an integer but you're passing invalid stuff. Otherwise its 
  // some hex number. In other words it goes into a 'fail' state when the 
  // expected input is not passed.
  cout << (cin >> n);

  char j;

  // This works normally (returns non-zero value) because expected input is char
  cout << (cin >> j);

  // -----------------------------------------------------------------------------
  // check what value scanf returns

  // scanf returns 1 if the thing entered is a number, otherwise 0
  cout << (scanf("%d", &n));

  // this works normally
  cout << (scanf("%c", &j));

  // -----------------------------------------------------------------------------
}