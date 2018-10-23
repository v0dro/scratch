#include <stdio.h>
enum ndt {
  Module, Fixed
};

struct _ndt {
  union {
    struct {
      char *name;
    } Module;
    
    struct {
      int x;
    } Fixed;
  };
};

int main()
{
  enum types {
    ZERO, ONE, TWO, THREE
  } types;

  printf("first :: %d\n", THREE);

  struct _ndt a;
  printf(a.Module.name);

  //  enum dytes {
  //ZERO, ONE, TWO, THREE
  //} dytes;

  //printf("second :: %d", dytes.ZERO);
  
}

