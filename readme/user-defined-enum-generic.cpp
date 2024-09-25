#include <iostream>

#include "../cpp-dump.hpp"

CPP_DUMP_DEFINE_EXPORT_ENUM_GENERIC(member_a, member_b, member_c);

int main() {
  std::clog << std::endl;

  enum class enum_A { member_a, member_b, member_c };

  enum_A my_enum_A = enum_A::member_c;
  cpp_dump(my_enum_A);

  std::clog << std::endl;
}
