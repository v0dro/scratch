#include <fftw3.h>
#include <iostream>
using namespace std;

int main(void)
{
  int N = 4;

  fftw_complex *in, *out;
  fftw_plan my_plan;
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  my_plan = fftw_plan_dft_1d(4, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  in[0][0] = 3.10;
  in[1][0] = 1.73;
  in[2][0] = 1.04;
  in[3][0] = 2.83;

  in[0][1] = 0;
  in[1][1] = 0;
  in[2][1] = 0;
  in[3][1] = 0;

  fftw_execute(my_plan);

  for (int i = 0; i < 4; ++i) {
    cout << out[i][0] << " " << out[i][1] << "i" << endl;
  }

  fftw_destroy_plan(my_plan);
  fftw_free(in);
  fftw_free(out);
  return 0;
}