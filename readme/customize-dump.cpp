#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "../cpp-dump.hpp"
namespace cp = cpp_dump;

void my_func() {
  std::map<int, int> my_map{{2, 6}, {4, 6}, {5, 3}};
  std::set<char> my_set{'A', 'p', 'p', 'l', 'e'};
  cpp_dump(my_map);
  cpp_dump(my_set);
}

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> my_vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};

  std::clog << "// Print the filename and line instead of [dump]" << std::endl;
  CPP_DUMP_SET_OPTION(log_label_func, cp::log_label::filename());

  cpp_dump(my_vector);
  my_func();

  std::clog << std::endl;

  std::clog << "// Print along with the function name" << std::endl;
  CPP_DUMP_SET_OPTION(log_label_func, cp::log_label::filename(true));

  cpp_dump(my_vector);
  my_func();

  std::clog << std::endl;
}
