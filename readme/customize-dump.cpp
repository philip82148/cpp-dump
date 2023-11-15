#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "../dump.hpp"

void my_func() {
  std::map<int, int> my_map{{2, 6}, {4, 6}, {5, 3}};
  std::set<char> my_set{'A', 'p', 'p', 'l', 'e'};
  cpp_dump(my_map);
  cpp_dump(my_set);
}

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> my_vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};

  std::clog << "// Print the filename instead of [dump]" << std::endl;
  cpp_dump::log_label_func = cpp_dump::log_label::filename();

  cpp_dump(my_vector);
  my_func();

  std::clog << std::endl;

  std::clog << "// Print along with the function name" << std::endl;
  cpp_dump::log_label_func = cpp_dump::log_label::filename(true);

  cpp_dump(my_vector);
  my_func();

  std::clog << std::endl;
}
