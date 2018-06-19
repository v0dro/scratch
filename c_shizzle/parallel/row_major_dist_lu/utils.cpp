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
    // for (int br = 0; br < block_size_per_process_r; ++br) {
    //   for (int bc = 0; bc < block_size_per_process_c; ++bc) {
    //     for (int i = 0; i < process_block_size; ++i) {
    //       for (int j = 0; j < process_block_size; ++j) {
    //         //int row_i = 
    //       }
    //     }
    //   }
    // }
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
