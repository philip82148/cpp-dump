#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

int main() {
  std::clog << std::endl;

  cpp_dump::max_line_width = 100;

  std::vector my_vector{
      "This is a test string.", "This is a test string.", "This is a test string."};
  CPP_DUMP(my_vector);

  my_vector.push_back("This is a test string.");
  CPP_DUMP(my_vector);

  std::clog << std::endl;
}
