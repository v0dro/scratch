// // basically want to calculate the force between two sets of particles that
// //   are far away from each other. So we first group them into a single box and
// //   then create interactions between these two boxes.

// // Plan is to first create two sets of particles that are far away from each other.

// // Then group each set into a single particle by simply adding their value of q.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DISTANCE 10

int main()
{
  int i, j, N = 10;
  double xi[N], yi[N], zi[N], ui[N];
  double xj[N], yj[N], zj[N], qj[N];
  double r, dx, dy, dz;

  for (i = 0; i < N; ++i)
  {
    xi[i] = DISTANCE + drand48();
    yi[i] = drand48();
    zi[i] = drand48();
    ui[i] = 0; // effect of j particles on i particles

    xj[i] = drand48();
    yj[i] = drand48();
    zj[i] = drand48();
    qj[i] = 1; // 1 cuz its easier to verify results. force of particles in j

    printf("xi: %lf yi: %lf zi: %lf ui: %lf\n", xi[i], yi[i], zi[i], ui[i]);
    printf("xj: %lf yj: %lf zj: %lf qj: %lf\n", xj[i], yj[i], zj[i], qj[i]);
  }

  // (I think) hypothetical sums of the distance vectors inside each box.
  double xip = 0 , yip =0, zip=0, uip;
  // 0.5 cuz the average of the points needs to lie in the geometrical centre.
  // xip = DISTANCE + 0.5;
  // yip = zip = 0.5;
  double xjp=0, yjp=0, zjp=0, qjp;
  // xjp = yjp = zjp = 0.5;

  for (i = 0; i < N; ++i)
  {
    xip += xi[i];
    yip += yi[i];
    zip += zi[i];

    xjp += xj[i];
    yjp += yj[i];
    zjp += zj[i];
  }

  // Step 1 in the diagram.
  qjp = 0;
  for (j = 0; j < N; ++j)
  {
    qjp += qj[j];
  }

  // Step 2
  // 10 is the distance between one set of points i and other set j.
  uip = qjp / DISTANCE;

  // Step 3
  for (i = 0; i < N; ++i)
  {
    ui[i] = uip;
  }

  // Check answer
  for (i = 0; i < N; ++i)
  {
    double uid = 0;
    for (j = 0; j < N; ++j)
    {
      dx = xi[i] - xj[j];
      dy = yi[i] - yj[j];
      dz = zi[i] - zj[j];
      r = sqrt(dx*dx + dy*dy + dz*dz);
      uid += qj[j] / r;
    }
    printf("ui[i]: %lf uid: %lf qj[i] %lf\n", ui[i], uid, qj[i]);
  }

  return 0;
}
