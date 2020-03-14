#include <unordered_map>
#include <functional>

enum class MAN { SAMEER, PETER, SAGAR, NEEL };

int main(int argc, char *argv[])
{
  MAN m = MAN::SAMEER;
  auto add_func = [](int a, int b) {
                    a + b;
                  };
  using func = std::function<void(int, int)>;

  std::unordered_map<const MAN, func> mapp({{MAN::SAMEER, add_func}});
  
  return 0;
}
