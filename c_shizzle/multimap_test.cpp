#include <unordered_map>
#include <map>
#include <tuple>
#include <iostream>
#include <cassert>
#include <chrono>

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
};

enum RowCol { ROW, COL };

int main(int argc, char* argv[]) {
  int N = atoi(argv[1]);
  int admis = 2;

  std::unordered_map<std::tuple<int, int>, bool> m;

  // https://www.geeksforgeeks.org/multimap-associative-containers-the-c-standard-template-library-stl/
  std::multimap< std::tuple<int, RowCol>, int> mm_admis, mm_inadmis;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (std::abs(i - j) <= admis) {
        m.insert({std::make_tuple(i, j), false});
      }
      else {
        m.insert({std::make_tuple(i, j), true});
      }
    }
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (std::abs(i - j) <= admis) {
        mm_inadmis.insert({std::make_tuple(i, ROW), j});
      }
      else {
        mm_admis.insert({std::make_tuple(i, ROW), j});
      }
    }
  }

  for (int j = 0; j < N; ++j) {
    for (int i = 0; i < N; ++i) {
      if (std::abs(i - j) <= admis) {
        mm_inadmis.insert({std::make_tuple(j, COL), i});
      }
      else {
        mm_admis.insert({std::make_tuple(j, COL), i});
      }
    }
  }

  auto start_map = std::chrono::high_resolution_clock::now();
  int map_sum = 0;
  // loop over map to find out a sum
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (m[{i, j}]) { // admis
        map_sum += 1;
      }
      else {
        map_sum += 5;
      }
    }
  }
  auto stop_map = std::chrono::high_resolution_clock::now();
  auto map_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_map - start_map).count();

  auto start_mm = std::chrono::high_resolution_clock::now();
  int mm_sum = 0;
  for (int i = 0; i < N; ++i) {
    // https://thispointer.com/finding-all-values-for-a-key-in-multimap-using-equals_range-example/
    auto iter_inadmis = mm_inadmis.equal_range({i, ROW});
    for (auto itr = iter_inadmis.first; itr != iter_inadmis.second; ++itr) {
      mm_sum += 5;
    }

    auto iter_admis = mm_admis.equal_range({i, ROW});
    for (auto itr = iter_admis.first; itr != iter_admis.second; ++itr) {
      mm_sum += 1;
    }
  }
  auto stop_mm = std::chrono::high_resolution_clock::now();
  auto mm_time = std::chrono::duration_cast<std::chrono::microseconds>(stop_mm - start_mm).count();

  assert(map_sum == mm_sum);

  std::cout << "N: " << N <<  " map: "<< map_time << " multi-map: " << mm_time << std::endl;
}
