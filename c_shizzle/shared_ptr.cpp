#include <memory>
#include <iostream>
#include <vector>
#include <tuple>

class Mover {
public:
  // https://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used
  std::shared_ptr<double> data;
  int size = -1;

  Mover() = default;

  ~Mover() = default;

  Mover(Mover&& A) = default;

  Mover(int num) {
    data = std::make_shared<double>(num);
    size = num;
    double *d = data.get();

    for (int i = 0; i < num; ++i) {
      d[i] = i;
    }
  }

  Mover& operator=(Mover&& A) {
    std::cout << "move assign\n";
    std::swap(size, A.size);
    std::swap(data, A.data);

    return *this;
  }

  std::tuple<Mover, Mover>
  move_this(int n1, int n2) {
    Mover obj_n1(n1), obj_n2(n2);

    return {std::move(obj_n1), std::move(obj_n2)};
  }

  void print() {
    for (int i = 0; i < size; ++i) {
      std::cout << data.get()[i] << " ";
    }
    std::cout << std::endl;
  }
};

void test_move_semantics() {
  std::cout << "TEST move semantics with shared_ptr\n";
  Mover m(100);

  Mover a, b;

  std::cout << "pre a.size: " << a.size << std::endl;

  std::tie(a, b) = m.move_this(3, 3);

  a.print();

  std::cout << "post a.size: " << a.size << std::endl;
}

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

  test_move_semantics();
  return 0;
}
