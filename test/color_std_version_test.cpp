#if __cplusplus >= 202002L
#if !defined(__clang__)
#include <source_location>
#else
#include <string>

struct source_location {
  static std::string current() { return "The compiler doesn't support this class."; };
};
#endif
#endif

#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using namespace cpp_dump;

int main() {
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
  // dummy
#if __cplusplus >= 202002L
  // dummy:next is line 40
  cpp_dump(source_location::current());
#endif
}
