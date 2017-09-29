// index all co-ordinates and return a morton index. 
// for this case we are subdividing the X and Y line into 8 sub divisions.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
  int i, j, N = 10; // N points
  double x[N], y[N], u[N], q[N];
  double r, dx, dy;
  int index[N];

  for (i = 0; i < N; ++i)
  {
    x[i] = drand48();
    y[i] = drand48();
    u[i] = 0; // effect of j particles on i particles
    q[i] = 1; // 1 cuz its easier to verify results. force of particles in j

    printf("x: %lf y: %lf\n", x[i], y[i]);
  }


  // decide which level - how many boxes to divide the domain into
  // we are dividing the domain successively by 2. we know that the right side
  //  of the square is 1 and the left is 0. so the middle will be 0.5 and so on.

  // first determine the co-ordinates of the point.

  // calculate a 2D index that represents the two numbers.
  int iX[2];

  // the 'level' of division tells us to what level we want to divide the square.
  //  so a level of 3 indicates that the square will divided 3 times which will
  //  lead to 8 sub disions.
  int level = 3;
  for (i = 0; i < N; ++i)
  {
    iX[0] = x[i] * 8; // multiply by 8 cuz 8 subdivisions need to happen
    iX[1] = y[i] * 8; // multiply by 8 cuz 8 subdivisions need to happen

    // just save so they're not destroyed.
    int iX2[2] = { iX[0], iX[1] };
    // we have 2 bits, which we left shift and add.

    // the way to contruct morton indexes basically tells us to divide the square
    //  level by level. so the outer loop drills down the levels.
    // the inner loop exists for looping over both dimensions X and Y.

    // we then successively += the index so that after the shifting (multiplications)
    //  and additions we will eventually arrive at the morton index of the number.
    get_index(&index[i], iX, level);

    // calculate the co-ordinates from the morton index.
    level = 0;
    int index2 = index[i];
    int d = 0;
    while (index2 > 0)
    { 
      // check each rightmost bit and collect them (x, y separately) together 
      //  to create the co-ordinate for each dimension.
      iX[d] += (index2 % 2) * (1 << level);
      // right shift index to get a new bit in the right most area for consideration.
      index2 >>= 1;
      // go to the next dimension.
      d = (d+1) % 2;
      // if back to the X co-ord, reset the level so that the addition procedure
      //  can begin again.
      if (d == 0) level++;
    }
  }


  return 0;
}

int get_index(int *index, int iX[2], int level)
{
  *index = 0;
  for (int l = 0; l < level; ++l)
  {
    for (int d = 0; d < 2; ++d)
    {
      // look at one bit in iX, the left shift it by 2l times + d.

      // % 2 because it gives you back the number of the first bit. its masking.
      //  it will tell you whether number is odd or even.
      *index += iX[d] % 2 << (2 * l + d);

      // right shift it again and restore it so that we can examine the next bit.
      iX[d] >>= 1;
    }
  }
}
