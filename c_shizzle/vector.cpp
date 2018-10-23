#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
  std::vector<double> a;
  std::cout << "a.size : " << a.size() << std::endl;
  
  a.resize(10);

  std::cout << "a.size : " << a.size() << std::endl;
  return 0;
}
