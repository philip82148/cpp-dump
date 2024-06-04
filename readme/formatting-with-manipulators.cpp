#include <iostream>
#include <vector>

#define DEBUGGING
#ifdef DEBUGGING

#include "../dump.hpp"
namespace cp = cpp_dump;

#else
#define cpp_dump(...)
#define CPP_DUMP_SET_OPTION(...)
#endif

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
  cpp_dump(some_huge_vector | cp::back(10) | cp::both_ends(10) | cp::dec(2));

  std::clog << std::endl;

  // omitting-a-vector.png
  cpp_dump(some_huge_vector | cp::back(10) | cp::both_ends(10));

  std::clog << std::endl;

  // int-style.png
  cpp_dump(some_huge_vector | cp::bin(8, 2) | cp::front(5) | cp::front(5));

  std::clog << std::endl;

  // cont-index.png
  cp::max_iteration_count = 5;
  cpp_dump(some_huge_vector | cp::dec(2) | cp::index());

  std::clog << std::endl;
}
