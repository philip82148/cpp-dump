#include "../dump.hpp"
#include "./odr_test.hpp"

template <typename... Args>
constexpr bool check_iota(Args... args) {
  int x = 0;
  return (... && (args == x--));
}

#define BUFFER(x)      -x
#define EXPAND_VA(...) _p_CPP_DUMP_EXPAND_VA(BUFFER, __VA_ARGS__)

int main() {
  // for ODR
  odr_test();
  cpp_dump(class_a(), class_b(), enum_a::s);

  // _p_CPP_DUMP_EXPAND_VA
  static_assert(check_iota(EXPAND_VA(0)));
  static_assert(check_iota(EXPAND_VA(0, 1)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)));
  static_assert(check_iota(EXPAND_VA(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)));
}
