#include <iostream>
#include <ostream>
#include <vector>

#include "../dump.hpp"

int main() {
  std::clog << std::endl;

  cpp_dump::max_line_width = 100;

  std::vector vector{"This is a test string.", "This is a test string.", "This is a test string."};
  CPP_DUMP(vector);

  vector.push_back("This is a test string.");
  CPP_DUMP(vector);

  std::clog << std::endl;
}
