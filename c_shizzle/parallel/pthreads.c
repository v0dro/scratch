#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

int glob_x;

void* foo(void *a)
{
  int *a_ptr = (int*)a;
  *a_ptr = (*a_ptr)*2;
  printf("in thread %d.\n", syscall(SYS_gettid));
  return NULL;
}

int main()
{
  int x = 2;
  glob_x = 1;
  pthread_t th1, th2, th3, th4;

  pthread_create(&th1, NULL, foo, &x);
  pthread_create(&th2, NULL, foo, &x);
  pthread_create(&th3, NULL, foo, &x);
  pthread_create(&th4, NULL, foo, &x);
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  pthread_join(th4, NULL);

  printf("x: %d", x);
}
