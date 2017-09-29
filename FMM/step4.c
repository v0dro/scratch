// index all co-ordinates and return a morton index. 
// for this case we are subdividing the X and Y line into 8 sub divisions.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int compare_ints(const void *a, const void *b)
{
  const int *da = (const int *)a;
  const int *db = (const int *)b;

  return (*da > *db) - (*da < *db);
}

int sort_and_prepare_offset(int *index, int N, int *offset)
{
  qsort(index, N, sizeof(int), compare_ints);
  int ncells = 1, ic;

  offset[0] = 0;
  ic = index[0];
  for (int i = 1; i < N; ++i)
  {
    if (ic != index[i])
    {
      offset[ncells] = i;
      ic = index[i];
      ncells++;
    }
  }

  return ncells;
}

int main()
{
  int i, j, N = 100; // N points
  double x[N], y[N], u[N], q[N];
  double r, dx, dy;
  int index[N], offset[N];

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
  }

  // array that denotes the offsets of each level in the Multipole array below.
  int levelOffset[4];
  levelOffset[0] = 0;
  int ncells = sort_and_prepare_offset(index, N, offset);
  int maxcells = compute_maxcells(level, levelOffset);

  double *Multipole = (double*)malloc(maxcells*sizeof(double));

  // P2M
  for (i = 0; i < ncells; ++i)
  {
    for (j = offset[i]; j < offset[i+1]; ++j)
    {
      // summing all the charges in a box and gaining a simplistic multipole.
      // using 'level' cuz we're at the bottom level right now (3).
      Multipole[index[j] + levelOffset[level]] += q[j];
    }
  }

  // M2M
  int l;
  for (l=level;l>=2;l--)
  {
    int nc = 1 << (2*l);
    // loop over all the levels from bottom to top using this.
    for(i = 0;i < nc; ++i)
    { 
      // want offset of the parent so l-1
      int ip = i/4 + levelOffset[l-1];
      // since we are on the M2M level we add all the previously computed
      //  multipoles and store them in the places where their parents are
      //  supposed to be (inside the same array).
      Multipole[ip] += Multipole[i+level];
    }
  }

  // M2L
  // calculate interactions between near and far boxes (i think)
  int ix, iy;
  for (int l = 2; l <= level; ++l)
  {
    int nc = 1 << (2*l);
    for (i = 0; i < nc; ++i)
    {
      get_IX(i/4, iX);
      int ix, iy;
      for (ix++ = -1; ix <= 1; ++ix)
      {
        for (iy = -1; iy <= 1;  ++iy)
        {
          int iX2[2];
          // min and max are used to factor in edge cases that arise out of the
          //  boxes appearing at the corners.
          iX2[0] = min(max(iX[0]+ix,0), (1<<(2*l)));
          iX2[1] = min(max(iX[1]+iy,0), (1<<(2*l)));

          if (l == 2)
            printf("%d %d %d\n", i, iX2[0], iX2[1]);
        }
      }
    }
  }
  free(Multipole);

  // offset is stored at the index of the morton matrix offset. so having the
  //  morton index lets you get the offset directly.
  find_neighbours(26);
  return 0;
}

int compute_maxcells(int level, int *levelOffset)
{
  int maxcells = 0;

  for (int i = 0; i <= level; ++i)
  {
    maxcells += (1 << (2*i));
    levelOffset[i+1] = maxcells;
  }

  return maxcells;
}

void find_neighbours(int index)
{
  int ix, iy, iX[2];

  get_IX(index, iX);

  printf("co-ordinates: %d %d\n", iX[0], iX[1]);
  for (ix = -1; ix <= 1; ++ix)
  {
    for (iy = 0; iy <= 1; ++iy)
    {
      printf("New: %d %d\n", iX[0]+ix, iX[1]+iy);
    }
  }
}

// calculate the co-ordinates from the morton index.
void get_IX(int index, int iX[2])
{
  int level = 0, d = 0;
  iX[0] = 0;
  iX[1] = 0;
  while (index > 0)
  { 
    // check each rightmost bit and collect them (x, y separately) together 
    //  to create the co-ordinate for each dimension.
    iX[d] += (index % 2) * (1 << level);
    // right shift index to get a new bit in the right most area for consideration.
    index >>= 1;
    // go to the next dimension.
    d = (d+1) % 2;
    // if back to the X co-ord, reset the level so that the addition procedure
    //  can begin again.
    if (d == 0) level++;
  }
}

void get_index(int *index, int iX[2], int level)
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
