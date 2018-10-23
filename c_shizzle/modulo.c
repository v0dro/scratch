#include <stdio.h>
#include <float.h>

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int main()
{
  int temp = -1;
  int s = 3;
  
  printf("-1 % 3: %d.\n -3 % 3: %d.\n", mod(temp, s), mod(-3, s));

  double a = 0x1.ffffffp+127;

  printf("a: %.10e. b: %.10e. isinf: %d. isinf: %d.\n", (float)a, a, isinf((float)a), isinf(a));
}
