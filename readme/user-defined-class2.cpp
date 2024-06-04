#include <iostream>

#define DEBUGGING
#ifdef DEBUGGING

#include "../dump.hpp"
namespace cp = cpp_dump;

#else
#define cpp_dump(...)
#define CPP_DUMP_SET_OPTION(...)
#define CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(...)
#endif

CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(i, str());

int main() {
  std::clog << std::endl;

  struct class_A {
    int i;
    std::string str() const { return std::to_string(i); }
  } my_class_A{10};

  cpp_dump(my_class_A);

  std::clog << std::endl;
}
