#include "../dump.hpp"
#include "./odr_test.hpp"

int main() {
  odr_test();
  cpp_dump(class_a(), class_b());
}
