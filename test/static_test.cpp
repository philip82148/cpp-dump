#include "../dump.hpp"
#include "./odr_test.hpp"

template <typename... Args>
constexpr bool check_iota(Args... args) {
  int x = 0;
  return (... && (args == x--));
}

#define MINUS(x)       -x
#define EXPAND_VA(...) _p_CPP_DUMP_EXPAND_VA(MINUS, __VA_ARGS__)

#define TO_16 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16

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
  static_assert(check_iota(EXPAND_VA(TO_16)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29)));
  static_assert(check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30))
  );
  static_assert(
      check_iota(EXPAND_VA(TO_16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31))
  );
}

template <>
void cpp_dump::write_log(std::string_view) {}
