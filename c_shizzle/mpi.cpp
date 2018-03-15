#include "mpi.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

void basic(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpisize, mpirank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  printf("rank: %d/%d\n",mpirank,mpisize);
  MPI_Finalize();
}

// Demo the BCast function. This function first computes an array containing some numbers
// and then broadcasts the contents of this array to ALL other processes.

// It can be seen that data from 0 (that is not populated) will be sent to data buffers in
// all other processes. Question: Will this happen to the corresponding buffer in the corresponding
// scope of the other processes.
void broadcast(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int data[4]; int i, mpi_size, mpi_rank;

  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if (mpi_rank != 0) {
    for (i = 0; i < 4; i++) data[i] = i +1;
  }
  printf("rank: %d: before : [%d %d %d %d]\n", mpi_rank, data[0], data[1], data[2], data[3] );
  MPI_Bcast(data, 4, MPI_INT, 0, MPI_COMM_WORLD);
  printf("rank: %d: after : [%d %d %d %d]\n", mpi_rank, data[0], data[1], data[2], data[3] );
  MPI_Finalize();
}

// MPI_Scatter sends chunks of an array to different processes. Unlike Broadcast, it does not send all
// data to all processes. It still sends data to all processes from the root process.
//
// Unlike Bcast it does not use the same array for sending and receiving data. It will divide the array
// into chunks of data and sends these chunks to all the processes.
//
// As can be seen in the below function, the send array for proc 0 is {1,2,3,4}. The data in each position
// will get copied into the recv buffer at the 0 position. If the send buffer was length 8 there would have
// been two elements getting copied (for 4 processes). Scatter will divide the number of elements with the
// number of processes.
void scatter(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int send[4] = {1,2,3,4}; int recv[4] = {0,0,0,0};
  if (mpi_rank != 0) {
    for (int i =0;i < 4; ++i) send[i] = i + 1;
  }
  MPI_Scatter(send, 1, MPI_INT, recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("rank%d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",mpi_rank,
         send[0],send[1],send[2],send[3], recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// MPI_Gather will gather chunks from data stored in buffers in other processors and gather
// them in the process that is specified as root.
//
// This data will be gathered in subdivided chunks. In below example, there is one data point
// from each buffer that is being sent.
void gather(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int send[4] = {0,0,0,0}, recv[4] = {0,0,0,0};
  send[0] = mpi_rank + 1;
  MPI_Gather(send, 1, MPI_INT, recv, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("rank%d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",
         mpi_rank, send[0],send[1],send[2],send[3], recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// AllGather makes an improvement over Gather and takes a chunk from a send buffer
// and distributes it to all the corresponding chunks of the other processes.
//
// It differs from Gather in that it collects data from all processes and broadcasts
// respective chunk from each process to all other processes.
void all_gather(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  int send[4] = {0,0,0,0}, recv[4] = {0,0,0,0};
  send[0] = mpi_rank + 1;
  MPI_Allgather(send, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);
  printf("rank %d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",
         mpi_rank, send[0],send[1],send[2],send[3], recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// Reduce is useful for getting elements from an array and reducing by the operation specified.
//
// It works by communicating the operation on the entire array to the root process.
//
// See this link for more: http://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/
// Question: how does it magically know which array position to put the result in?
void reduce(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int send[4], recv[4] = {0};
  for (int i=0; i<4; i++) send[i] = mpi_rank + i;
  
  MPI_Reduce(send, recv, 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  printf("rank %d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",
         mpi_rank,send[0],send[1],send[2],send[3], recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// Works like Allgather. Reduces the send array and sends the computed data to all the other processes.
void all_reduce(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int send[4], recv[4] = {0};
  for (int i=0; i<4; i++) send[i] = mpi_rank + i;
  
  MPI_Allreduce(send, recv, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("rank%d: send=[%d %d %d %d], recv[%d %d %d %d]\n",
         mpi_rank,send[0],send[1],send[2],send[3], recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// Alltoall sends data from all processes to all procesess. The nth element in the send
// array is send to the nth process from each process. Below output looks like so:
// rank0: send=[0 10 20 30], recv=[0 1 2 3]
// rank1: send=[1 11 21 31], recv=[10 11 12 13]
// rank2: send=[2 12 22 32], recv=[20 21 22 23]
// rank3: send=[3 13 23 33], recv=[30 31 32 33]
void all2all(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpisize, mpirank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize); MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  int send[4] = {0,0,0,0}, recv[4] = {0,0,0,0};

  for(int i=0; i<4; i++) send[i] = mpirank+10*i;
  MPI_Alltoall(send, 1, MPI_INT, recv, 1, MPI_INT, MPI_COMM_WORLD);
  printf("rank%d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",
         mpirank, send[0],send[1],send[2],send[3],recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

// This function uses Send and Recv functions for sending and receiving data from processes.
//
// http://mpitutorial.com/tutorials/mpi-send-and-receive/
void send_recv(int argc, char ** argv)
{
  MPI_Init(&argc, &argv); int mpisize, mpirank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  int send[4] = {0,0,0,0}, recv[4] = {0,0,0,0};

  for(int i=0; i<4; i++) send[i] = mpirank+10*i;
  
  if (mpirank == 0) {
    MPI_Send(send, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (mpirank==1) {
    MPI_Recv(recv, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  printf("rank%d: send=[%d %d %d %d], recv=[%d %d %d %d]\n",
         mpirank, send[0],send[1],send[2],send[3],recv[0],recv[1],recv[2],recv[3]);
  MPI_Finalize();
}

void struct_comm(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  typedef struct {
    int a;
    float b;
    char c;
  } HELLO;

  MPI_Aint intex, floatex, charex;
  MPI_Type_extent(MPI_INT, &intex);
  MPI_Type_extent(MPI_FLOAT, &floatex);
  MPI_Type_extent(MPI_CHAR, &charex);

  HELLO a;
  MPI_Datatype MPI_CUSTOM; // name of the new struct data type.
  int structlen = 3;
  int block_lengths[structlen] = {1,1,1};
  int displacements[structlen];
  MPI_Datatype types[3] = {MPI_INT, MPI_FLOAT, MPI_CHAR};
  displacements[0] = (MPI_Aint) 0;
  displacements[1] = intex;
  displacements[2] = intex + floatex;
  
  
  MPI_Finalize();
};

void dynamic_array(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  double *a;
  int s = 10;

  if (mpi_rank == 0) {
    a = (double*)malloc(s*sizeof(double));
    for (int i = 0; i < s; ++i) {
      a[i] = i;
    }
    MPI_Send(a, s, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
  }
  else {
    double *c = (double*)malloc(s*sizeof(double));
    MPI_Recv(c, s, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < s; ++i) {
      cout << c[i] << endl;
    }
  }

  MPI_Finalize();
}

void std_vec(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_size, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  vector<double> a;
  int s = 10;
  
  if (mpi_rank == 0) {
    a.resize(s);
    for (int i = 0; i < s; ++i) {
      a[i] = i;
    }
    MPI_Send(a.data(), s, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
  }
  else {
    double *c = (double*)malloc(s*sizeof(double));
    MPI_Recv(c, s, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < s; ++i) {
      cout << c[i] << endl;
    }
  }

  MPI_Finalize();
}

void vector_pointers()
{
  vector<double> b;
  b.push_back(1);
  b.push_back(2);
  vector<vector<double>* > a;
  a.push_back(&b);
  cout << (*a[0])[1];
}

void split_range(int & begin, int & end, int iSplit, int numSplit) {
  //assert(end > begin);                                      // Check that size > 0
  int size = end - begin;                                   // Size of range
  int increment = size / numSplit;                          // Increment of splitting                 
  int remainder = size % numSplit;                          // Remainder of splitting    
  begin += iSplit * increment + std::min(iSplit,remainder); // Increment the begin counter
  end = begin + increment;                                  // Increment the end counter
  if (remainder > iSplit) end++;                            // Adjust the end counter for remainder
}

void matmul(double *a, double *b, double *c, int nrows_a, int ncols_a, int nrows_b, int ncols_b)
{
  cblas_dgemm(
              CblasRowMajor, CblasNoTrans, CblasNoTrans,
              nrows_a, ncols_b, nrows_b,
              1, a, ncols_a, b,
              ncols_b, 1, c, ncols_b);
}


void generate_rand_array(double *x, int nrows) {
  srand(time(NULL));
  for (int i = 0; i < nrows; ++i) {
    x[i] = (double)rand();
  }
}

void summa(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  int N = sqrt(mpi_size), NROWS = 10, NCOLS = 10;
  double *a; double *b; double *c;
  int row_rank = mpi_rank / N;
  int col_rank = mpi_rank % N;
  double x[NROWS];
  generate_rand_array(x, NROWS);
  int nrows = NROWS/N;
  int ncols = NCOLS/N;

  a = (double*)malloc(nrows*ncols*sizeof(double));
  b = (double*)malloc(nrows*ncols*sizeof(double));
  c = (double*)calloc(nrows*ncols, sizeof(double));
  for (int i = 0; i < nrows; ++i) {
    for (int j = 0; j < ncols; ++j) {
      a[i*nrows + j] = 1 / abs(x[i + col_rank*ncols] - x[j + col_rank*ncols] - ncols);
      b[i*nrows + j] = 1;
    }
  }

  // send a to all cols in a given row.
  for (int c = 0; c < N; ++c) {
    int proc_number = row_rank*N + c;
    MPI_Send(&a, nrows*ncols, MPI_DOUBLE, proc_number, 0, MPI_COMM_WORLD);
  }

  // send b to all rows in a given col.
  for (int r = 0; r < N; ++r) {
    int proc_number = r*N + col_rank;
    MPI_Send(&b, nrows*ncols, MPI_DOUBLE, proc_number, 1, MPI_COMM_WORLD);
  }

  MPI_Status s;

  // get numbers from both rows and columns and add them to the product.
  for (int i = 0; i < N; ++i) {
    int proc_number_r = row_rank*N + i;
    int proc_number_c = i*N + col_rank;
    MPI_Recv(&a, nrows*ncols, MPI_DOUBLE, proc_number_r, 0, MPI_COMM_WORLD, &s);
    MPI_Recv(&b, nrows*ncols, MPI_DOUBLE, proc_number_c, 1, MPI_COMM_WORLD, &s);
    c += a*b;
  }

  cout << "r: " << row_rank << " c: " << col_rank << " c: " << c << endl;

  MPI_Finalize();
}

int main(int argc, char**argv)
{

  // basic(argc, argv);
  //  broadcast(argc, argv);
  // scatter(argc, argv);
  // gather(argc, argv);
  // all_gather(argc, argv);
  //reduce(argc, argv);
  //  all_reduce(argc, argv);
  //all2all(argc, argv);
  // send_recv(argc, argv);
  //dynamic_array(argc, argv);
  //std_vec(argc, argv);
  // vector_pointers();
  summa(argc, argv);
}
