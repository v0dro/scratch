#include <iostream>
using namespace std;

class A
{ 
  int j;
public:
  A(int h)
  {
    j = h;
  }

  int
  get()
  {
    return j;
  }
};

void
hui(A d)
{
  cout << "j  " << d.get();
}

int main()
{
  hui(A(23));
  return 0;
}