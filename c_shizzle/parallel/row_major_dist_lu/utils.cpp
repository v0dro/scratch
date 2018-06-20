#include "utils.hpp"

void initialize_blacs(int *BLACS_CONTEXT, int *proc_nrows, int *proc_ncols,
                      int *myrow, int *mycol, int *proc_id, int *num_procs)
{
  *proc_nrows = 2; *proc_ncols = 2;
  Cblacs_pinfo(proc_id, num_procs);
  Cblacs_get( -1, 0, BLACS_CONTEXT );
  Cblacs_gridinit( BLACS_CONTEXT, "Row", *proc_nrows, *proc_ncols );
  Cblacs_pcoord(*BLACS_CONTEXT, *proc_id, myrow, mycol);
}

void generate_data(double *a, int myrow, int mycol, desc desc_a)
{
  int pblock_nrows = desc_a.MB/2;
  int pblock_ncols = desc_a.NB/2;
  int num_blocks_per_process = 4;
  //int process_block_size = 
  
  if (COL_MAJOR) {
    // loop over block cols
    for (int bc = 0; bc < pblock_ncols; ++bc) {
      // loop over block rows
      for (int br = 0; br < pblock_nrows; ++br) {
        // loop over number cols
        for (int j = 0; j < pblock_ncols; ++j) {
          // loop over number rows
          for (int i = 0; i < pblock_nrows; ++i) {
            int row_i = myrow*pblock_nrows + i + j * num_blocks_per_process;
            int col_i = bc*num_blocks_per_process + mycol * pblock_ncols + br;
            int index = i + j * pblock_nrows +
              (br + bc * pblock_ncols) * num_blocks_per_process;
            a[index]  = row_i + col_i*desc_a.N;
          }
        }
      }
    }
  }
  else if (ROW_MAJOR) {
    if (myrow == 0 && mycol == 0) {
      a[0] = 0; a[1] = 1; a[2] = 4; a[3] = 5;
      a[4] = 8; a[5] = 9; a[6] = 12; a[7] = 13;
      a[8] = 32; a[9] = 33; a[10] = 36; a[11] = 37;
      a[12] = 40; a[13] = 41; a[14] = 44; a[15] = 45;
    }
    else if (myrow == 0 && mycol == 1) {
      a[0] = 2; a[1] = 3; a[2] = 6; a[3] = 7;
      a[4] = 10; a[5] = 11; a[6] = 14; a[7] = 15;
      a[8] = 34; a[9] = 35; a[10] = 38; a[11] = 39;
      a[12] = 42; a[13] = 43; a[14] = 46; a[15] = 47;
    }
    else if (myrow == 1 && mycol == 0) {
      a[0] = 16; a[1] = 17; a[2] = 20; a[3] = 21;
      a[4] = 24; a[5] = 25; a[6] = 28; a[7] = 29;
      a[8] = 48; a[9] = 49; a[10] = 52; a[11] = 53;
      a[12] = 56; a[13] = 57; a[14] = 60; a[15] = 61;
    }
    else if (myrow == 1 && mycol == 1) {
      a[0] = 18; a[1] = 19; a[2] = 22; a[3] = 23;
      a[4] = 26; a[5] = 27; a[6] = 30; a[7] = 31;
      a[8] = 50; a[9] = 51; a[10] = 54; a[11] = 55;
      a[12] = 58; a[13] = 59; a[14] = 62; a[15] = 63;
    }
  }
}

void print_block(double *A, int nb, int nc, int myrow, int mycol, char *desc)
{
  cout << desc;
  int start = 0;
  double *temp = &A[start];

  for (int i = 0; i < nc*nc; ++i) {
    cout << temp[i] << " ";
  }

  cout << endl;
}

void print_files(double *A, int nrows, int ncols, int myrow, int mycol, string postfix /*=""*/)
{
  string n = to_string(myrow*2 + mycol); 
  ofstream file;

  file.open(n + postfix + ".txt");
  print_arr(A, nrows*ncols, n, file);
  file.close();
}

void print_arr(double *A, int size, string desc, ostream &o)
{
  for (int i = 0; i < size; ++i) {
    o << A[i] << " ";
  }
  o << endl;
}

void print_mat(double *A, int nrows, int ncols, int myrow, int mycol, string desc, ostream &o)
{
  o << desc << endl;
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      o << "(" << nrows*myrow + i << "," <<
        ncols*mycol + j << ") " << A[i*ncols + j] << " ";
    }
    o << endl;
  }
  o << endl;
}
