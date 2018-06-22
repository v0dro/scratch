#include "unit_tests.hpp"

void test_global2local()
{
  desc desc_a;
  
  desc_a.M = 8;
  desc_a.N = 8;
  desc_a.MB = 4;
  desc_a.NB = 4;
  desc_a.BLACS_CONTEXT = -1;
  desc_a.rsrc = 0;
  desc_a.csrc = 0;
  desc_a.lld = 4;
  
  int global, local;
  int diag = 3;
  int num_procs = 4;
  global = diag*desc_a.N + diag;

  global2local(global, &local, num_procs, desc_a);
  assert(local == 5);
}
