#include <iostream>
using namespace std;
// find the number of ways you can combine three sticks such 
// that a triangle cannot be made from them

int main(void)
{
  int N;
  long int* stick_lengths;
  int output[10000];

  while (1) {
    cin >> N;
    if (N == 0) break;

    stick_lengths = new long int[N];

    for (int i = 0; i < N; ++i) { cin >> stick_lengths[i]; }
    // have an array of stick lengths
    // want to determine how many triangles can be 
  }
}