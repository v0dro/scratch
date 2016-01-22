#include <iostream>
#include "omp.h"
using namespace std;

#define MAX 10000

int search(int* array, int start, int end, int num)
{ 
  for (int i = start; i < end; ++i) {
    if (array[i] == num) {
      return i;
    }
  }

  return 0;
}

int main()
{ 
  int procs = omp_get_num_procs();
  int *parts = new int[procs + 1];
  int array[MAX];
  int num = 100;
  int answer = 0, final;

  for (int i = 0; i < MAX; ++i) {
    array[i] = i * 3 - 2;
  }

  int step = MAX/procs;
  for (int i = 0; i <= procs; ++i) {
    parts[i] = step*i;
  }

  if (parts[procs] == MAX - 1) { parts[procs] = MAX; } 

  #pragma omp parallel num_threads(procs)
  {
    int ID = omp_get_thread_num();
    answer = search(array, parts[ID], parts[ID + 1], num);
    if (answer != 0) { final = answer; }
  }

  cout << "value found at : " << final << endl;
}
