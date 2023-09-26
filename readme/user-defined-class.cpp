#include <iostream>
#include <ostream>

#include "../dump.hpp"

struct my_class {
  int i;
  std::string str() const { return std::to_string(i); }
};
CPP_DUMP_DEFINE_EXPORT_OBJECT(my_class, i, str());

int main() {
  std::clog << std::endl;

  my_class new_my_class{10};
  CPP_DUMP(new_my_class);

  std::clog << std::endl;
}
