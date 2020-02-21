#include <iostream>
#include <atomic>

template <typename T>
void foo(T& a, T val) {
  std::atomic<T> bar(a);
  T old = bar;
  while (!bar.compare_exchange_weak(old, old +val));
}

int main(int argc, char *argv[])
{
  auto q = 22;
  foo(q, 11);
  std::cout << "foo: " << q << std::endl;
  return 0;
}
