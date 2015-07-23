#include <stdio.h>
#define LENGTH 10

struct stack 
{
  int top;
  int elements[LENGTH];
} stack;

void
push(int num)
{
  if (stack.top == LENGTH) {
    cout << "Stack is full. Do something!\n";
  }

  stack.top++;
  stack.elements[stack.top] = num;
}

int
top()
{
  return stack.elements[stack.top];
}

int
empty()
{
  if (stack.top == -1) return 1;

  return 0;
}

void
pop()
{ 
  int popped;

  if (stack.empty()) {
    cout << "Stack is empty.\n";
  }
  popped = stack.elements[stack.top];
  stack.top--;

  return popped;
}

void
print(long int numbers[])
{
  int i;

  for (i = 0; i < LENGTH; ++i) {
    printf("%ld ", numbers[i]);
  }
}

void
quick_sort(long int numbers[], int leftlower, int rightupper)
{ 
  int leftupper, rightupper;
  
  if (start < end) {
    partition(numbers, leftlower, rightupper, leftupper, rightlower);
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