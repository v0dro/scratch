#include <iostream>

int main(int argc, char *argv[])
{
  double a = 1e-300;
  double b = 1e-301;
    std::cout << "num: " << a << " a-b: " << a-b << std::endl;
  return 0;
}
