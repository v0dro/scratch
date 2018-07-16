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

void init_mpi(int *myrow, int *mycol, int *proc_id, int *num_procs,
              int *proc_nrows, int *proc_ncols)
{ 
  *proc_nrows = 2; *proc_ncols = 2;

  MPI_Comm_rank(MPI_COMM_WORLD, proc_id);
  MPI_Comm_size(MPI_COMM_WORLD, num_procs);

  *myrow = (int)((*proc_id) / (*proc_nrows));
  *mycol = (*proc_id) % (*proc_ncols);
}

void generate_data(double *a, int block_size_per_process_r,
                   int block_size_per_process_c, int process_block_size,
                   int num_blocks_per_process, int myrow, int mycol, int N)
{
  if (COL_MAJOR) {
    // loop over block cols
    for (int bc = 0; bc < block_size_per_process_c; ++bc) {
      // loop over block rows
      for (int br = 0; br < block_size_per_process_r; ++br) {
        // loop over number cols
        for (int j = 0; j < process_block_size; ++j) {
          // loop over number rows
          for (int i = 0; i < process_block_size; ++i) {
            int row_i = myrow*process_block_size + i + j * num_blocks_per_process;
            int col_i = bc*num_blocks_per_process + mycol * process_block_size + br;
            int index = i + j * process_block_size +
              (br + bc * process_block_size) * num_blocks_per_process;
            a[index]  = row_i + col_i*N;
          }
        }
      }
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
