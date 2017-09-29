// Near far decomposition in a 3D cube like structure.
// We don't actually make the boxes for storing the numbers since its easier to
//  simply hypotheically position the points as inside a box and figuring out
//  which points are near and which points are far.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
  int i, j, N = 4; // N points
  double xi[N], yi[N], zi[N], ui[N*64];
  double xj[N], yj[N], zj[N], qj[N];
  double r, dx, dy, dz;

  for (i = 0; i < N; ++i)
  {
    xi[i] = drand48();
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
  xip = 0.5;
  yip = zip = 0.5;
  double xjp=0, yjp=0, zjp=0, qjp;
  xjp = yjp = zjp = 0.5;

  // Step 1 in the diagram.
  qjp = 0;
  for (j = 0; j < N; ++j)
  {
    qjp += qj[j];
  }

  // Step 2. Need boxes in a 3D cube that are atleast 2 spaces away from each other.
  int ix, iy, iz, jx, jy, jz;

  // * * * #
  // * # * #
  // * * * #
  // # # # #
  // ^^ near field. Above is far.

  // Below is a simulation of a box. So we just add the ix to the x and so on in
  //  order to create a full cube that contains these points.
  for (ix = 0; ix < 4; ix++) {
    for (iy = 0; iy < 4; iy++) {
      for (iz = 0; iz < 4; iz++) {
        // why multiply by N?
        // Since you're adding ui to each POINT, the box number should be multiplied
        //  by the number of points so that you can can the index of each point
        //  inside each box by looping.
        int ibox = (ix + iy*4 + iz*16)*N;
        uip = 0;
        for (jx = 0; jx < 4; jx++) {
          for (jy = 0; jy < 4; jy++) {
            for (jz = 0; jz < 4; jz++) {
              if (abs(ix - jx) < 2 && abs(iy - jy) < 2 && abs(iz - jz) < 2) {
                // this is the near field.
                for(i =0;i < N; i++) { // bodies inside the target blue box.
                  // these are bodies inside the red boxes that this box is being
                  //  compared with.
                  for(j=0;j<N;j++) {
                    // below additions simulate co-ordinates inside a box.
                    //  ix,jx style values.
                    dx = xi[i] + ix - xj[j] - jx;
                    dy = yi[i] + iy - yj[j] - jy;
                    dz = zi[i] + iz - zj[j] - jz;
                    r = sqrt(dx*dx + dy*dy + dz*dz);

                    if (r != 0)
                      ui[i+ibox] += qj[j] / r;
                  }
                }
              }
              else {
                //  Step 2.
                // this is far field. We're just dealing with the center of the
                // boxes and not the particles within.
                dx = ix - jx;
                dy = iy - jy;
                dz = iz - jz;
                r = sqrt(dx*dx + dy*dy + dz*dz);
                uip += qjp / r; // accumulate cuz many far boxes for near ones.
              }
            } 
          } 
        }
        // Step 3. 
        // This takes place for each box. So we iterate on the bodies present
        //  inside each box and 
        for (i = 0; i < N; i++) {
          ui[i+ibox] += uip;
        }
      } 
    }    
  }

  for (ix = 0; ix < 4; ix++) {
    for (iy = 0; iy < 4; iy++) {
      for (iz = 0; iz < 4; iz++) {
        int ibox = (ix + iy*4 + iz*16)*N;
        for (i = 0; i < N; i++) {
          double uid = 0;
          for (jx = 0; jx < 4; jx++) {
            for (jy = 0; jy < 4; jy++) {
              for (jz = 0; jz < 4; jz++) {
                for (j = 0; j < N; j++) {
                  dx = xi[i] + ix - xj[j] - jx;
                  dy = yi[i] + iy - yj[j] - jy;
                  dz = zi[i] + iz - zj[j] - jz;
                  r = sqrt(dx*dx + dy*dy + dz*dz);
                  if (r!=0)
                    uid += qj[j]/r;
                }
              } 
            }
          }  
          printf("uid: %lf ui[i+ibox] %lf i+ibox: %d\n", uid, ui[i+ibox],i+ibox);
        }
      } 
    }     
  }

  return 0;
}
