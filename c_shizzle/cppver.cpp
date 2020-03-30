#include <iostream>

int main(int argc, char *argv[])
{
  std::cout << "cpp version: " << __cplusplus << std::endl;
  std::cout << "double : " << sizeof(double) << std::endl;
  std::cout << "float: " << sizeof(float) << std::endl;
  std::cout << "int: " << sizeof(int) << std::endl;
  return 0;
}
