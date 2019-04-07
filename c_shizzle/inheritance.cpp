// program to test inheritance of declared variables in CPP
#include <iostream>

class A {
public:
  int num;

  int n() {
    return num;
  };
};

class B : public A {
public:
  B() {
    num = 3;
  }
};

int main(int argc, char *argv[])
{
  B a;

  std::cout << "a.num= " << a.n() << std::endl;
  return 0;
}
