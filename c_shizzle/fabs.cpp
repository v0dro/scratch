#include <cstdlib>
#include <cmath>
#include <iostream>
#include <limits>

int main(int argc, char **argv) {
  std::cout << "fabs: " << fabs(-0.33) << " eps: "<<
    std::numeric_limits<double>::epsilon() <<
    "diff: " << 1.66533453693773e-16 - std::numeric_limits<double>::epsilon() << std::endl;
}
