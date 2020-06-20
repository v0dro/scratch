#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>

class Scalar {
public:
  Scalar() {};
};

auto scalar_assign = [](auto * self_data, Scalar src_data) {
                       using scalar_t = typename std::remove_pointer<decltype(self_data)>::type;
                       // *self_data = src_data;
                     };

auto f(int n)
{
    if (n == 1)
        return 1;
 
    return 2.0;
}
class ScalarMapGenerator {
public:
  template <typename lhs_type>
  auto operator()(){
    // using lhs_type = int*;
    using binary_func_t = std::function<void(lhs_type, Scalar)>;
    std::vector<lhs_type> a;
    return a;
  }

  template<typename T>
  void hello() {
    T a;
  }
};

int main(int argc, char *argv[])
{
  auto m = ScalarMapGenerator();
  m.hello<int>();
  auto h = m.operator()<Scalar>();
  
  return 0;
}


