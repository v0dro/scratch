#include <unordered_map>
#include <map>
#include <tuple>
#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>

namespace std {
  template <>
  struct hash<std::tuple<int, int> > {
    size_t operator()(const std::tuple<int, int>& pair) const {
      int64_t first, second;
      std::tie(first, second) = pair;
      size_t first_hash = hash<int>()(first);
      first_hash ^= (hash<int>()(second) + 0x9e3779b97f4a7c17 +
                     (first_hash << 6) + (first_hash >> 2));
      return first_hash;
    }
  };

  template <>
  struct hash<std::tuple<int, int, int>> {
    size_t operator()(const std::tuple<int, int, int>& pair) const {
      int first, second, third;
      std::tie(first, second, third) = pair;
      size_t first_hash = hash<int>()(first);
      size_t second_hash = first_hash ^ (hash<int>()(second) + 0x9e3779b97f4a7c17 +
                                         (first_hash << 6) + (first_hash >> 2));
      size_t third_hash = second_hash ^ (hash<int>()(third) + 0x9e3779b97f4a7c17 +
                                         (second_hash << 6) + (second_hash >> 2));
      return third_hash;
    }
  };
};

// ➜  c_shizzle git:(master) ✗ ./a.out 1000
// multi map: 391 vector: 8

int main(int argc, char * argv[]) {
  int N = atoi(argv[1]);

  std::unordered_map<std::tuple<int, int, int>, int> mm;
  std::vector<int> vec;


  for (int i = 0; i < N; ++i) {
    mm.insert({{i,i+1,i+2}, i * 100});
    vec.push_back(i * 100);
  }

  double ss = 0;
  auto start_mm = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    ss += mm[{i,i+1,i+2}];
  }
  auto stop_mm = std::chrono::high_resolution_clock::now();
  auto mm_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_mm - start_mm).count();

  double vv = 0;
  auto start_vec = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; ++i) {
    vv += vec[i];
  }
  auto stop_vec = std::chrono::high_resolution_clock::now();
  auto vec_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_vec - start_vec).count();

  assert(vv == ss);

  std::cout << "multi map: " << mm_time << " vector: " << vec_time << std::endl;
}
