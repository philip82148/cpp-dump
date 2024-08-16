#include <iostream>
#include <string>
#include <vector>

#include "../cpp-dump.hpp"

int main() {
  std::clog << std::endl;

  CPP_DUMP_SET_OPTION(max_line_width, 100);

  std::vector<std::string> my_vector(3, "This is a test string.");
  cpp_dump(my_vector);

  my_vector.push_back("This is a test string.");
  cpp_dump(my_vector);

  std::clog << std::endl;
}
