#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};
  CPP_DUMP(vector);

  std::clog << std::endl;
}
