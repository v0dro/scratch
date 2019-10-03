#include <vector>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
  std::vector<double> a;
  std::cout << "a.size : " << a.size() << std::endl;
  
  a.resize(10);

  std::cout << "a.size : " << a.size() << std::endl;

  std::vector<double> * s = new std::vector<double>;
  return 0;
}
