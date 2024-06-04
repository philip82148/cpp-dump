#include <iostream>

#define DEBUGGING
#ifdef DEBUGGING

#include "../dump.hpp"
namespace cp = cpp_dump;

#else
#define cpp_dump(...)
#define CPP_DUMP_SET_OPTION(...)
#define CPP_DUMP_DEFINE_EXPORT_ENUM(...)
#endif

enum class enum_A { a, b, c };
CPP_DUMP_DEFINE_EXPORT_ENUM(enum_A, enum_A::a, enum_A::b, enum_A::c);

int main() {
  std::clog << std::endl;

  enum_A my_enum_A = enum_A::c;
  cpp_dump(my_enum_A);

  std::clog << std::endl;
}
