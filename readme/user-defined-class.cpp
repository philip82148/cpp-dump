#include <iostream>
#include <string>

#include "../cpp-dump.hpp"

struct class_A {
  int i;
  std::string str() const { return std::to_string(i); }
};
CPP_DUMP_DEFINE_EXPORT_OBJECT(class_A, i, str());

int main() {
  std::clog << std::endl;

  class_A my_class_A{10};
  cpp_dump(my_class_A);

  std::clog << std::endl;
}
