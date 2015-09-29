/* Name: Sameer Deshmukh
 * Class: BE I
 * Roll: 405199
 * Title: Program for implementing linear and binary search
 */

#include <stdio.h>
#define ELEMENTS 10
#define SORTED 1
#define NOT_SORTED 0

int
linear_search(int num[], int number_of_elements, int value)
{
  int i;

  for (i = 0; i < number_of_elements; ++i) {
    if (num[i] == value) return i;
  }
  return -1;
}

void
print(int num[], int number_of_elements)
{
  int i;

  printf("\n");

  for (i = 0; i < number_of_elements; ++i) {
    printf(" %d ", num[i]);
  }
  printf("\n");
}

void
sort_array(int num[], int number_of_elements)
{
  int i, j, temp;

  for (i = 0; i < ELEMENTS; ++i) {
    for (j = i; j < ELEMENTS; ++j) {
      if (num[i] > num[j]) {
        temp   = num[i];
        num[i] = num[j];
        num[j] = temp;
      }
    }
  }
}

int
binary_search(int num[], int number_of_elements, int sorted, int value)
{
  int lower_index = 0, 
      upper_index = number_of_elements -1, 
      mid;

  if (!sorted) {
    sort_array(num, number_of_elements);
  }

  while (lower_index < upper_index) {
    mid = (lower_index + upper_index) / 2;
    if (num[mid] == value) {
      return mid;
    }
    else if (num[mid] < value) {
      lower_index = mid + 1;
    }
    else {
      upper_index = mid - 1;
    }
  }
  
  return -1;
}

void
accept_array_of_length(int num[], int number_of_elements)
{
  int i;

  printf("\nInput an array of length %d\n", number_of_elements);
  for (i = 0; i < number_of_elements; ++i) {
    scanf("%d", &num[i]);
  }
}

int
accept_single_value(const char which[6])
{
  int value;
  printf("Input single %s\n", which);
  scanf("%d", &value);

  return value;
}

int main()
{ 
  int num[10];
  int value;

  accept_array_of_length(num, ELEMENTS);
  value = accept_single_value("value");

  printf("==============LINEAR SEARCH==============\n");
  printf("Linear search returns result: %d\n", 
    linear_search(num, ELEMENTS, value));
  
  printf("==============BINARY SEARCH==============\n");
  printf("Binary search returns result: %d\n", 
    binary_search(num, ELEMENTS, NOT_SORTED, value));

  return 0;
}

/* OUTPUT
Input an array of length 10
54
235
1
55
345
11
-235
315
1245
552
Input single value
11
==============LINEAR SEARCH==============
Linear search returns result: 5
==============BINARY SEARCH==============
Binary search returns result: 2
*/
