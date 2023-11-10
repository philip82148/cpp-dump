#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

namespace mn = cpp_dump::mn;

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> some_huge_vector(100, std::vector<int>(100));

  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      some_huge_vector[i][j] = ((i + 1) * 7 % 10) * ((j + 1) * 3 % 10);
    }
  }

  // default: max_iteration_count = 16;
  cpp_dump(some_huge_vector);

  std::clog << std::endl;

  cpp_dump(mn::show_back(10) << mn::show_both_ends(10) << some_huge_vector);

  std::clog << std::endl;

  cpp_dump(mn::int_style(2, 8, 2) << mn::show_front(5) << mn::show_front(5) << some_huge_vector);

  std::clog << std::endl;
}
