#include <memory>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
  std::vector<double> v(10);
  v[0] = 1;
  std::shared_ptr<std::vector<double> > p = std::make_shared<std::vector<double> >(v);
  std::shared_ptr<std::vector<double> > a = p;
  std::shared_ptr<std::vector<double> > b;

  std::cout << "before resize:\n";
  std::cout << "&p[0]= " << &(*p.get())[0] << std::endl;

  std::cout << "post copy:\n";
  std::cout << "&a[0]= " << &(*a.get())[0] << std::endl;

  b = a;
  (*a.get()).resize(100);
  std::cout << "post resize:\n";
  std::cout << "&a[0]= " << &(*a.get())[0] << std::endl;
  std::cout << "&p[0]= " << &(*p.get())[0] << std::endl;
  std::cout << "&b[0]= " << &(*b.get())[0] << std::endl;

  std::vector<double> w(10);
  w[0] = 3;

  std::shared_ptr<std::vector<double> > x = std::make_shared<std::vector<double> >(w);
  p.reset();

  std::cout << "post x:\n";
  std::cout << "&x[0]= " << &(*x.get())[0] << std::endl;
  std::cout << "&p[0]= " << &(*p.get())[0] << std::endl;
  std::cout << "&b[0]= " << &(*b.get())[0] << std::endl; 
  return 0;
}
