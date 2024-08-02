#include <typeindex>

#include "../dump.hpp"

namespace cp = cpp_dump;

namespace ns {

struct class_a {};

}  // namespace ns

int main() {
  CPP_DUMP_SET_OPTION(
      es_value,
      (cp::es_value_t{
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
      })
  );

  cpp_dump(typeid(void));

  CPP_DUMP_SET_OPTION(max_line_width, 2000);

  std::type_index type_index1 = typeid(ns::class_a);
  cpp_dump(typeid(ns::class_a), type_index1);

  CPP_DUMP_SET_OPTION(max_line_width, 20);

  cpp_dump(typeid(ns::class_a), type_index1);

  CPP_DUMP_SET_OPTION(max_depth, 0);

  cpp_dump(typeid(ns::class_a), type_index1);
}
