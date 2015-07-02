#include <iostream>
using namespace std;
// find the number of ways you can combine three sticks such 
// that a triangle cannot be made from them

void
sort_array(long int arr[], int length)
{
  long int temp;

  for (int i = 0; i < length; ++i) {
    for (int j = i; j < length; ++j) {
      if (arr[i] > arr[j]) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
}

int
invalid_triangle_traids(long int stick_lengths[], int a, int b, int N)
{
  long int sum = stick_lengths[a] + stick_lengths[b];
  int c = b + 1;

  while (sum >= stick_lengths[c]) {
    c++;
    if (c == N) break;
  }

  return (N - c);
}

int main(void)
{
  int N, a, b, c;
  long int* stick_lengths;
  int output[1000], op_index = 0;

  while (1) {
    cin >> N;
    if (N == 0) break;
    output[op_index] = 0;

    stick_lengths = new long int[N];

    for (int i = 0; i < N; ++i) { cin >> stick_lengths[i]; }
    
    sort_array(stick_lengths, N);
    for (int a = 0; a < N-2; ++a) {
      for (int b = a + 1; b < N-1; ++b) {
        output[op_index] += invalid_triangle_traids(stick_lengths, a, b, N); 
      }
    }

    op_index++;
  }

  for (int i = 0; i < op_index; ++i) {
    cout << output[i] << endl;
  }
}