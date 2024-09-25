#include <iostream>

#include "../cpp-dump.hpp"

enum class enum_A { member_a, member_b, member_c };
CPP_DUMP_DEFINE_EXPORT_ENUM(enum_A, enum_A::member_a, enum_A::member_b, enum_A::member_c);

int main() {
  std::clog << std::endl;

  enum_A my_enum_A = enum_A::member_c;
  cpp_dump(my_enum_A);

  std::clog << std::endl;
}
