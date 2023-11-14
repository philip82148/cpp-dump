#include "./odr_test.hpp"

#include "../dump.hpp"

int main() {
  odr_test();
  cpp_dump(class_a(), class_b());
}
