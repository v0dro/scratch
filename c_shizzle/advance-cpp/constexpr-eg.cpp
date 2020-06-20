#include <iostream>
#include <cstdlib>

// why can this be made constexpr even though the values inside it are not being kept const?
// 
// if the function is supposed to be a constant expression that can be evaluated at compile
// time, why is it that when passing totally dynamic inputs that are determined from the command
// line, I can still make it constexpr?
class Func {
  int member;
public:
  constexpr void operator() (double &left, double &right) const {
    left += right;
  };
};

int main(int argc, char *argv[])
{
  Func f;
  double a = atof(argv[1]), b = atof(argv[2]);

  f(a, b);
  // f.hello(a);
  std::cout << "a: " << a << std::endl;
  
  
  return 0;
}
