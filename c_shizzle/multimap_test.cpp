#include <unordered_map>
#include <map>
#include <tuple>

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

int main() {
  int N = 100;
  int admis = 2;

  std::unordered_map<std::tuple<int, int>, bool> m;
  std::multimap< std::tuple<int, RowCol>, int> mm_admis, mm_inadmis;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (std::abs(i - j) <= admis) {
        m.insert({std::make_tuple(i, j), true});
      }
      else {
        m.insert({std::make_tuple(i, j), false});
      }
    }
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (std::abs(i - j) <= admis) {
        mm_admis.insert({std::make_tuple(i, ROW), j});
      }
      else {
        mm_inadmis.insert({std::make_tuple(i, ROW), j});
      }
    }
  }

  for (int j = 0; j < N; ++j) {
    for (int i = 0; i < N; ++i) {
      if (std::abs(i - j) <= admis) {
        mm_admis.insert({std::make_tuple(j, COL), i});
      }
      else {
        mm_inadmis.insert({std::make_tuple(j, COL), i});
      }
    }
  }
}
