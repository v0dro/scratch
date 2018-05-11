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

void generate_data(double *a, int block_size_per_process_r,
                   int block_size_per_process_c, int process_block_size,
                   int num_blocks_per_process, int myrow, int mycol, int N)
{
  for (int bcounter_i = 0; bcounter_i < block_size_per_process_r; ++bcounter_i) {
    for (int bcounter_j = 0; bcounter_j < block_size_per_process_c; ++bcounter_j) {
      for (int i = 0; i < process_block_size; ++i) {
        for (int j = 0; j < process_block_size; ++j) {
          int row_i = bcounter_i*num_blocks_per_process +
            myrow*block_size_per_process_r + i;
          int col_j = bcounter_j*num_blocks_per_process +
            mycol*block_size_per_process_c + j;
          int index = (bcounter_i*block_size_per_process_r + bcounter_j)*
            num_blocks_per_process +  i*process_block_size + j;
          a[index] = row_i*N + col_j;
        }
      }
    }
  }
}
