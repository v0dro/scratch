#include <unordered_map>
#include <functional>
#include <iostream>
#include <chrono>

class F {
public:
  constexpr void operator() (int & a, int & b) const {
    a += b;
  }
};

int main(int argc, char* argv[]) {
  F fun;
  
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  std::chrono::time_point<std::chrono::system_clock> start, stop;
  auto time = 0.0;
  using binary_t = std::function<void(int&, int&)>;
  // constexpr std::unordered_map<int, binary_t> funcs[] = {
  //                                                        {0, fun}
  // };
  
  funcs[0] = fun;
  start = std::chrono::system_clock::now();
  for (long long i = 0; i < 10000000; ++i) {
    funcs[0](a, b);
  }
  stop = std::chrono::system_clock::now();
  
  std::cout << "time with unordered_map: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
  std::cout << "a: " << a << std::endl;
  a = 1;

  start = std::chrono::system_clock::now();
  for (long long i = 0; i < 10000000; ++i) {
    fun(a, b);
  }
  stop = std::chrono::system_clock::now();
  
  std::cout << "time without map: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
  std::cout << "a: " << a << std::endl;
}
