#include <stdio.h>
#include <stdlib.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_spline2d.h>

int main(int argc, char const *argv[])
{
  printf("%d\n", gsl_interp2d_bilinear);
  printf("%d\n", gsl_interp2d_bicubic);

  return 0;
}