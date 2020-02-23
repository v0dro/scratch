#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
using namespace std;

const int N = 10;
const int M = 3;
const int LDA = 10;
int src[N * M], index[N * M], output[N * M];

mutex global;

void scatter_add(int dim) {
  lock_guard<mutex> guard(global);

  for (int i = 0; i < N; ++i) {
    output[index[dim * LDA + i] * LDA + i] += src[dim * LDA + i];
  }

}

int main(int argc, char *argv[])
{
  vector<thread> threads;

  for (int i = 0; i < N * M; ++i) {
    src[i] = 1;
    index[i] = 0;
    output[i] = 0;
  }
  
  for (int dim = 0; dim < 3; ++dim) {
    threads.push_back(thread(scatter_add, dim));
  }

  for (int dim = 0; dim < 3; ++dim) {
    threads[dim].join();
  }

  std::cout << "output:\n";
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      std::cout << output[i * LDA + j] << " ";
    }
    std::cout << "\n";
  }

  return 0;
}
