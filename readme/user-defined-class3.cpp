#include <iostream>

#define DEBUGGING
#ifdef DEBUGGING

#include "../dump.hpp"
namespace cp = cpp_dump;

#else
#define cpp_dump(...)
#define CPP_DUMP_SET_OPTION(...)
#endif

struct class_A {
  int i;
  std::string str() const { return std::to_string(i); }
};

std::ostream &operator<<(std::ostream &os, const class_A &a) {
  os << "class_A{ i= " << a.i << ", str()= \"" << a.str() << "\" }";
  return os;
}

int main() {
  std::clog << std::endl;

  class_A my_class_A{10};
  cpp_dump(my_class_A);

  std::clog << std::endl;
}
