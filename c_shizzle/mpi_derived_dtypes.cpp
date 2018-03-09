#include "mpi.h"
#include <cstdlib>
#include <boost/any.hpp>
#include <vector>

#define MASTER 0

class Dense
{
public:
  int dim[2];
  double *data;

  Dense() {};
  
  Dense(int nrows, int ncols)
  {
    dim[0] = nrows;
    dim[1] = ncols;
    data = (double*)malloc(nrows*ncols*sizeof(double));
    for (int i = 0; i < nrows; ++i) {
      for (int j = 0; j < ncols; ++j) {
        data[i*ncols + j] = i + j;
      }
    }
  };

  Dense(int nrows, int ncols, double * d)
  {
    dim[0] = nrows;
    dim[1] = ncols;
    data = (double*)malloc(nrows*ncols*sizeof(double));

    for (int i = 0; i < nrows; ++i) {
      for (int j = 0; j < ncols; ++j) {
        data[i*ncols +j] = d[i*ncols + j];
      }
    }
  };
  
  void print(char * desc)
  {
    printf("matrix: %s\n.", desc);
    for (int i = 0; i < dim[0]; ++i) {
      for (int j = 0; j < dim[1]; ++j) {
        printf(" %3.4f ", data[i*dim[1] + j]);
      }
      printf("\n");
    }
  }
};

class LR
{
public:
  Dense U, S, V;
  int dim[2];
  int rank;

  LR() {};
  
  LR(Dense U, Dense S, Dense V, int rank, int nrows, int ncols)
  {
    this->U = U;
    this->S = S;
    this->V = V;
    this->dim[0] = nrows;
    this->dim[1] = ncols;
    this->rank = rank;
  }
};

class H {
public:
  int dim[2];
  std::vector<boost::any> data;

  H(int nrows, int ncols) {
    dim[0] = nrows;
    dim[1] = ncols;
    data.resize(4);
  };

 void set_data(Dense &d00, LR &lr01, LR &lr10, Dense &d11)
  {
    data[0] = d00;
    data[1] = lr01;
    data[2] = lr10;
    data[3] = d11;
  };
};

int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  int rank = 5;
  int nrows = 10, ncols = 10, nr, nc;
  int dense_tag = 0;
  int row_tag = 1;
  int col_tag = 2;
  int sender, receiver;
  
  Dense d00(nrows, ncols);
  Dense d11(nrows, ncols);
  Dense u(nrows, rank);
  Dense s(rank, rank);
  Dense v(rank, ncols);
  LR lr01(u, s, v, rank, nrows, ncols);
  LR lr10(u, s, v, rank, nrows, ncols);

  H mat(nrows*2, ncols*2);
  mat.set_data(d00, lr01, lr10, d11);

  MPI_Request dim_reqs[4];
  MPI_Status stats[4];

  if (mpi_rank == 0) {
    MPI_Isend(&nrows, 1, MPI_INT, 1, row_tag, MPI_COMM_WORLD, &dim_reqs[2]);
    MPI_Isend(&ncols, 1, MPI_INT, 1, col_tag, MPI_COMM_WORLD, &dim_reqs[3]);
    MPI_Waitall(2, &dim_reqs[2], &stats[2]);
  }
  
  if (mpi_rank == 1) {
    MPI_Irecv(&nr, 1, MPI_INT, 0, row_tag, MPI_COMM_WORLD, &dim_reqs[0]);
    MPI_Irecv(&nc, 1, MPI_INT, 0, col_tag, MPI_COMM_WORLD, &dim_reqs[1]);
    MPI_Waitall(2, dim_reqs, stats);
  }

  printf("rank: %d nr: %d nc: %d nrows: %d ncols: %d\n", mpi_rank, nr, nc, nrows, ncols);

  // int next , prev ,buf[2] , tag1 =1 , tag2 =2;
  // tag1= tag2 = 0;
  // MPI_Request reqs[ 4 ] ;
  // MPI_Status stats[ 4 ] ;

  // int rank, numtasks;

  // int nrows, ncols;

  // prev = rank -1;
  // next =  rank +1;
  // if ( rank == 0 ) prev = numtasks - 1 ;
  // if (rank == numtasks -1) next = 0;

  // MPI_Irecv(&buf[0], 1, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
  // MPI_Irecv(&buf[1], 1, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[1]);
  // MPI_Isend(&rank, 1, MPI_INT, next, tag2, MPI_COMM_WORLD, &reqs[2]);
  // MPI_Isend(&rank, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);
  // MPI_Waitall(4, reqs, stats);
  // printf("t %d comm with tasks %d %d\n", rank, prev, next);
  
  // MPI_Request dim_reqs[4], data_reqs[2];
  
  // MPI_Status dim_stats[4], data_stats[2];

  // printf("rank %d size: %d\n", mpi_rank, mpi_size);
  // 1. send and receive dense data.
  // simple example: send and received a double array with irecv and isend
    //}


  // if (mpi_rank == MASTER) d00.print("d00");

  // int nr, nc;
  // double *d;
    
  // MPI_Isend(&nrows, 1, MPI_INT, 1, row_tag, MPI_COMM_WORLD, &dim_reqs[0]);
  // MPI_Isend(&ncols, 1, MPI_INT, 1, col_tag, MPI_COMM_WORLD, &dim_reqs[1]);
  // MPI_Irecv(&nr, 1, MPI_INT, 0, row_tag, MPI_COMM_WORLD, &dim_reqs[2]);
  // MPI_Irecv(&nc, 1, MPI_INT, 0, col_tag, MPI_COMM_WORLD, &dim_reqs[3]);
  //   if (mpi_rank != MASTER) printf("nr: %d nc: %d", nr, nc);
  // MPI_Waitall(4, dim_reqs, dim_stats);
  

  
  // //if (mpi_rank != MASTER) {
  // d = (double*)malloc(sizeof(double)*nr*nc);

  // MPI_Irecv(d, nr*nc, MPI_DOUBLE, 0, dense_tag, MPI_COMM_WORLD, &data_reqs[1]);
  // MPI_Isend(d00.data, nrows*ncols, MPI_DOUBLE, 1, dense_tag, MPI_COMM_WORLD, &data_reqs[0]);
  // MPI_Waitall(2, data_reqs, data_stats);
  
  // Dense copy(nr, nc, d);
  // copy.print("copy");
    //}

  double *a;
  a = (double*) malloc(..);
  
  MPI_Finalize();
}
