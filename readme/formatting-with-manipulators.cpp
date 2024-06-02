#include <iostream>
#include <vector>

#include "../dump.hpp"

namespace cp = cpp_dump;

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

  // manipulators.png
  cpp_dump(some_huge_vector | cp::show_back(10) | cp::show_both_ends(10) | cp::dec(2));

  std::clog << std::endl;

  // omitting-a-vector.png
  cpp_dump(some_huge_vector | cp::show_back(10) | cp::show_both_ends(10));

  std::clog << std::endl;

  // int-style.png
  cpp_dump(some_huge_vector | cp::bin(8, 2) | cp::show_front(5) | cp::show_front(5));

  std::clog << std::endl;

  // cont-index.png
  cp::max_iteration_count = 5;
  cpp_dump(some_huge_vector | cp::dec(2) | cp::cont_index());

  std::clog << std::endl;
}
