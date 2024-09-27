#include <iostream>
#include <string>

#include "../cpp-dump.hpp"

CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(i, str());

int main() {
  std::clog << std::endl;

  struct class_A {
    int i;
    std::string str() const { return std::to_string(i); }
  } my_class_A{10};

  cpp_dump(my_class_A);

  std::clog << std::endl;
}
