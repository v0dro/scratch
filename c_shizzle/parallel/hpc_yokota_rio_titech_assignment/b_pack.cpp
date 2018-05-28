#include <iostream>
#include "config.hpp"
using namespace std;

#define NR 5
#define KC 5
#define MR 5
#define NC 10
#define MC 10

#define N 20

void pack_for_b(int *packB, int *B)
{
  int *temp_packb = packB;
  for(int i = 0; i < MC; )
}

int main()
{
  int b[N*N], a[N*N];
  int packb[KC*MC];
  int packa[NC*KC];

  for (int i=0; i<N*N; ++i) {
    b[i] = i+1;
    a[i] = i+1;
  }

  print_mat(b, N, N, "b is this:\n");
  pack_for_b(packb, b);
  print_mat(packb, KC, MC, "packb is this:\n");
}
