#include <typeindex>

#include "../dump.hpp"

int main() {
  std::type_index type_index1 = typeid(void);
  cpp_dump(typeid(void), type_index1);
}
