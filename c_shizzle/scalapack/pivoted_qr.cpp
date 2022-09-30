extern "C" {
  /* Cblacs declarations */
  void Cblacs_pinfo(int*, int*);
  void Cblacs_get(int, int, int*);
  void Cblacs_gridinit(int*, const char*, int, int);
  void Cblacs_pcoord(int, int, int*, int*);
  void Cblacs_gridexit(int);
  void Cblacs_barrier(int, const char*);

  int numroc_(int*, int*, int*, int*, int*);

  // init descriptor for scalapack matrices.
  void descinit_(int *desc, const int *m,  const int *n, const int *mb,
                 const int *nb, const int *irsrc, const int *icsrc, const int *BLACS_CONTEXT,
                 const int *lld, int *info);
  // Pivoted QR. A is replaced with elementary reflectors of A.
  void	pdgeqpf_(const int* m, const int* n, double* a, const int* ia,
                 const int* ja, const int* desca, int* ipiv, double* tau,
                 double* work, const int* lwork, int* info);

  // Obtain QR factors from elementary reflectors obtained from geqpf.
  void	pdorgqr_(const int* m, const int* n, const int* k,
                 double* a, const int* ia, const int* ja,
                 const int* desca, const double* tau, double* work,
                 const int* lwork, int* info);
}

int MYROW, MYCOL, MPISIZE, MPIRANK;
int MPIGRID[2];

int zero = 0;

int main(int argc, char** argv) {
  // MPI init
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &MPISIZE);
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIRANK);
  MPI_Dims_create(MPISIZE, 2, MPIGRID);
  // end MPI Init

  // BLACS init
  int BLACS_CONTEXT;

  Cblacs_get(-1, 0, &BLACS_CONTEXT );
  Cblacs_gridinit(&BLACS_CONTEXT, "Row", MPIGRID[0], MPIGRID[1]);
  Cblacs_pcoord(BLACS_CONTEXT, MPIRANK, &MYROW, &MYCOL);

  int N = 100, NB = 20;
  int A_lrows = numroc_(&N, &NB, &MYROW, &zero, &MPIGRID[0]);
  int A_lcols = numroc_(&N, &NB, &MYCOL, &zero, &MPIGRID[1]);
  double* a = (double*)malloc(sizeof(double)*A_lrows*A_lcols);

  MPI_Finalize();
}
