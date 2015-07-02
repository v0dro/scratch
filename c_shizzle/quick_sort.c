#include <stdio.h>
#define LENGTH 10

void
print(long int numbers[])
{
  int i;

  for (i = 0; i < LENGTH; ++i) {
    printf("%ld ", numbers[i]);
  }
}

void
quick_sort(long int numbers[], int start, int end)
{ 
  if (start < end) {
    partition(numbers, start, end);
    quick_sort(numbers, )
  }
}

int 
main(void)
{
  long int num[LENGTH] = {34,66,21,7,134,73,23,1,453,45};
  int i;

  printf("=========ARRAY BEFORE SORT=========\n");
  print(num);

  quick_sort(num, 0, LENGTH-1);

  printf("=========ARRAY AFTER SORT==========\n");
  print(num);
}