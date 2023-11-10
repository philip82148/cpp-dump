#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> my_vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};

  cpp_dump::log_label_func = cpp_dump::log_label::filename();
  cpp_dump(my_vector);

  std::clog << std::endl;
}
