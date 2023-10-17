#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

int main() {
  std::clog << std::endl;
  using namespace cpp_dump;

  std::vector<std::vector<int>> some_huge_vector(100, std::vector<int>(100));

  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      some_huge_vector[i][j] = ((i + 1) * 7 % 10) * ((j + 1) * 3 % 10);
    }
  }

  // default: max_iteration_count = 16;
  CPP_DUMP(some_huge_vector);

  std::clog << std::endl;

  CPP_DUMP(show_back(10) << show_both_ends(10) << some_huge_vector);

  std::clog << std::endl;

  CPP_DUMP(int_style(2, 8, 2) << show_front(5) << show_front(5) << some_huge_vector);

  std::clog << std::endl;
}
