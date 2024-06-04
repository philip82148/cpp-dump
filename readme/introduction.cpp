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

  std::vector<std::vector<int>> my_vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};
  cpp_dump(my_vector);

  std::clog << std::endl;
}
