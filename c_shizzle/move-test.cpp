#include <vector>
#include <iostream>

void test_move(std::vector<double> &view) {
  std::vector<double> a = std::move(view);

  std::cout << "&a[0]= " << &a[0] << std::endl;
}

int main(int argc, char *argv[])
{
  std::vector<double> a = {1,2,3,4,5};
  //  std::vector<double> b = std::move(a);

  std::cout << "main &a[0]=" << &a[0] << std::endl;
  test_move(a);

  // for (int i = 0; i < b.size(); ++i) {
  //   std::cout << b[i] << " ";
  // }
  // std::cout << std::endl;

  // std::vector<double> c(3);
  // std::move(b.begin() + 2, b.begin()+5, c.begin());


  // std::cout << "c.size= " << c.size() << std::endl;
  // for (int i = 0; i < c.size(); ++i) {
  //   std::cout << c[i] << " ";    
  // }
  // std::cout << std::endl;

  // std::cout << "a[0] = " << a[0] << std::endl;
  
  return 0;
}
