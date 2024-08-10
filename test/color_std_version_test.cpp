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
namespace cp = cpp_dump;

int main() {
  CPP_DUMP_SET_OPTION(
      es_value,
      (cp::types::es_value_t{
          "\x1b[02m",                // log: dark
          "\x1b[38;2;86;154;214m",   // expression:
          "\x1b[36m",                // reserved: cyan
          "\x1b[38;2;181;206;168m",  // number:
          "\x1b[38;2;215;152;61m",   // character:
          "\x1b[38;2;150;150;150m",  // op: dark2
          "\x1b[32m",                // identifier:  green
          "\x1b[38;2;156;220;254m",  // member: light blue
          "\x1b[31m",                // unsupported: red
          {
              "\x1b[33m",  // bracket_by_depth[0]: yellow
              "\x1b[35m",  // bracket_by_depth[1]: magenta
          },
          "\x1b[38;2;139;191;139m",  // class_op: white closer to green
          "\x1b[38;2;217;242;255m",  // member_op: white closer to light blue
          "",                        // number_op: default
          "\x1b[38;2;203;186;113m"   // escaped_char:
      })
  );

  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //

#if __cplusplus >= 202002L
  CPP_DUMP_SET_OPTION(max_line_width, 2000);

  cpp_dump(source_location::current());

  CPP_DUMP_SET_OPTION(max_line_width, 20);

  cpp_dump(source_location::current());

  CPP_DUMP_SET_OPTION(max_depth, 0);

  cpp_dump(source_location::current());
#endif
}
