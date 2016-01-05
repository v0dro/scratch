#include <malloc.h>
#include <fftw3.h>
#include <iostream>
using namespace std;

int main(void)
{
  const int nu[1] = {9};
  double* in = (double*)fftw_malloc(sizeof(double)*9);
  // in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  fftw_complex *out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*9);
  // in = (double*) fftw_malloc(sizeof(double)*N);
  // out = (double*) fftw_malloc(sizeof(double)*N);
  const fftw_r2r_kind k[2] = {FFTW_REDFT10, FFTW_REDFT11};
  fftw_plan my_plan = fftw_plan_dft_r2c_1d(9, in, out,FFTW_ESTIMATE);

  in[0] = 9.32;
  in[1] = 43.00;
  in[2] = 3.20;
  in[3] = 4.00;
  in[4] = 5.32;
  in[5] = 3.20;
  in[6] = 4.00;
  in[7] = 5.32;
  in[8] = 45.32;

  // in[0][1] = 0;
  // in[1][1] = 0;
  // in[2][1] = 0;
  // in[3][1] = 0;
  // in[4][1] = 0;
  // in[5][1] = 0;
  // in[6][1] = 0;
  // in[7][1] = 0;
  // in[8][1] = 0;

  fftw_execute(my_plan);

  cout << "output :: \n";
  for (int i = 0; i < 9; ++i) {
    cout << out[i][0] << " " << out[i][1] << "i" << endl;
  }

  // cout << "\n\nflags:: \n";
  // cout << "\nestimate : " << FFTW_ESTIMATE;
  // cout << "\nmeasure : " << FFTW_MEASURE;
  // cout << "\n exhaustive : " << FFTW_EXHAUSTIVE;
  // cout << "\npatient : " << FFTW_PATIENT;
  // cout << "\nwisdom only : " << FFTW_WISDOM_ONLY;
  // cout << "\ndestroy_ip : " << FFTW_DESTROY_INPUT;
  // cout << "\npreserve_ip : " << FFTW_PRESERVE_INPUT;
  // cout << "\nunaligned : " << FFTW_UNALIGNED;

// cout << "output\n";
// cout << FFTW_REDFT00 << endl;
// cout << FFTW_REDFT10 << endl;
// cout << FFTW_REDFT01 << endl;
// cout << FFTW_REDFT11 << endl;
// cout << FFTW_RODFT00 << endl;
// cout << FFTW_RODFT10 << endl;
// cout << FFTW_RODFT01 << endl;
// cout << FFTW_RODFT11 << endl;
// cout << "r2hc " << FFTW_R2HC << endl;
// cout <<  "hc2r " << FFTW_HC2R << endl;
// cout << "dht " << FFTW_DHT << endl;


  fftw_destroy_plan(my_plan);
  return 0;
}