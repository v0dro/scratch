#include "unit_tests.hpp"

void init_desc_a(desc &desc_a)
{
  desc_a.M = 8;
  desc_a.N = 8;
  desc_a.MB = 4;
  desc_a.NB = 4;
  desc_a.BLACS_CONTEXT = -1;
  desc_a.rsrc = 0;
  desc_a.csrc = 0;
  desc_a.lld = 4;
}

void init_mpi(mpi_desc &mpi)
{
  mpi.MP = 2;
  mpi.NP = 2;
  mpi.num_procs = 4;
  mpi.myrow = 1;
  mpi.mycol = 0;
}

void test_global2local()
{
  desc desc_a;
  init_desc_a(desc_a);

  int global, local;
  int diag = 3;
  int num_procs = 4;
  global = diag*desc_a.N + diag;

  global2local(global, &local, num_procs, desc_a);
  assert(local == 5);
}

void test_local2global()
{
  desc desc_a;
  init_desc_a(desc_a);
  
  int global, local = 14;
  int myrow = 0, mycol = 0, num_procs = 4;

  local2global(local, &global, myrow, mycol, num_procs, desc_a);
  assert(global == 44);

  myrow = 1; mycol = 1; local = 14;
  local2global(local, &global, myrow, mycol, num_procs, desc_a);
  assert(global == 62);  
}

void test_procg2l()
{
  desc desc_a;
  init_desc_a(desc_a);
  mpi_desc mpi;
  init_mpi(mpi);
  int grow = 3, gcol = 5, newrow, newcol;

  procg2l(grow, gcol, &newrow, &newcol, desc_a, mpi);
  assert(newrow == 1);
  assert(newcol == 0);
}

void test_g2l()
{  
  desc desc_a;
  init_desc_a(desc_a);
  mpi_desc mpi;
  init_mpi(mpi);
  int grow = 3, gcol = 5, lrow, lcol;

  g2l(grow, gcol, lrow, lcol, desc_a, mpi);
  assert(lrow == 1);
  assert(lcol == 3);

  grow = 4; gcol = 5;
  g2l(grow, gcol, lrow, lcol, desc_a, mpi);
  assert(lrow == -1);
  assert(lcol == -1);
}
