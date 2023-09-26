#include <iostream>
#include <ostream>

#include "../dump.hpp"

enum class my_enum { a, b, c };
CPP_DUMP_DEFINE_EXPORT_ENUM(my_enum, my_enum::a, my_enum::b, my_enum::c);

int main() {
  std::clog << std::endl;

  my_enum new_my_enum = my_enum::c;
  CPP_DUMP(new_my_enum);

  std::clog << std::endl;
}
