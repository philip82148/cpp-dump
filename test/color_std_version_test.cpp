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
  cpp_dump::es_value = {
      "\x1b[02m",                // log: dark
      "\x1b[38;2;86;154;214m",   // expression:
      "\x1b[36m",                // reserved: cyan
      "\x1b[38;2;181;206;168m",  // number:
      "\x1b[38;2;215;152;61m",   // character:
      "",                        // op: default
      "\x1b[32m",                // identifier:  green
      "\x1b[38;2;156;220;254m",  // member:
      "\x1b[31m",                // unsupported: red
      {
          "\x1b[33m",  // bracket_by_depth[0]: yellow
          "\x1b[35m",  // bracket_by_depth[1]: magenta
      },
  };

#if __cplusplus >= 202002L
  CPP_DUMP_SET_OPTION(max_line_width, 2000);

  cpp_dump(source_location::current());

  CPP_DUMP_SET_OPTION(max_line_width, 20);

  cpp_dump(source_location::current());

  CPP_DUMP_SET_OPTION(max_depth, 0);

  cpp_dump(source_location::current());
#endif
}
