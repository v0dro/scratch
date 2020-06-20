#include <iostream>

struct OPERATOR {
  struct REDUCE {
    enum {
          ADD, SUBTRACT
    };
  };
  struct POINT {
    enum {
          ASSIGN
    };
  };
};

void foo(struct OPERATOR op) {
  std::cout << op << std::endl;
}
int main(int argc, char *argv[])
{
  struct OPERATOR::REDUCE op; 
  // foo(OPERATOR::REDUCE);
  // foo(OPERATOR::POINT::ASSIGN);
  return 0;
}
