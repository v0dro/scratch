// author: @v0dro

#include <iostream>
#include <omp.h>
#include <cstdlib>
using namespace std;

class QSort
{
  int *array;
  int length;

public:
  QSort(int* a, int len) 
  {
    this->array = a;
    this->length = len;
  };

  void 
  sort()
  {
    quick_sort(0, length - 1);
  }

private:
  void
  quick_sort(int low, int high)
  { 
    if (low < high) {
      int piv_index = partition(low, high);
      #pragma omp parallel sections
      {
        #pragma omp section
        {
          quick_sort(low, piv_index-1);
        }
        #pragma omp section
        {
          quick_sort(piv_index+1, high);
        }
      }      
    }
  }

  int
  partition(int low, int high)
  {
    int i = low;
    int j = high;
    int piv = array[low];

    do {
      do {
        i += 1;
      } while(array[i] < piv and i <= high);

      while(array[j] > piv and j >= low) {
        j -= 1;
      }

      if (i < j) {
        swap_(i, j);
      }
    } while(i < j);

    array[low] = array[j];
    array[j] = piv;

    return j;
  }

  void
  swap_(int i, int j)
  {
    int temp;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
};

int 
main(int argc, char const *argv[])
{
  int *arr;  
  int N = 50;
  arr = new int[N];

  for (int i = 0; i < N; ++i) {
    arr[i] = rand() % 100;
  }
  QSort sorter(arr, N);
  sorter.sort();

  for (int i = 0; i < N; ++i) {
    cout << arr[i] << "\n";
  }

  return 0;
}