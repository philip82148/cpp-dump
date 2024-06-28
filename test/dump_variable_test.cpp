#include <array>
#include <unordered_map>
#include <unordered_set>
//
#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
namespace cp = cpp_dump;

CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(member_var, member_func());

int main(int argc, char *argv[]) {
  if (argc != 4) return 1;
  bool compiler_dependent = static_cast<bool>(stoi(argv[1]));
  auto es_style_ = (array{
      cp::es_style_t::no_es,
      cp::es_style_t::original,
      cp::es_style_t::by_syntax,
  }[stoi(argv[2])]);
  bool color_test = stoi(argv[3]);

  CPP_DUMP_SET_OPTION(es_style, es_style_);

  if (color_test)
    CPP_DUMP_SET_OPTION(
        es_value,
        (cp::es_value_t{
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
        })
    );

  if (compiler_dependent) {
    class original_error : public logic_error {
      using logic_error::logic_error;
    } original_error1("This is an original error.");

    struct original_class {
      int member_var = 5;
      std::string member_func() const { return "This is a member_func."; }
    };

    struct unsupported_original_class {
      int member_var = 5;
      std::string member_func() { return "This is a member_func."; }
    };

    cpp_dump(original_error1);
    cpp_dump(original_class());
    cpp_dump(unsupported_original_class());
  } else {
    // pointer
    const void *void_ptr = (void *)0x7ffd06586204;
    cpp_dump(void_ptr);

    // unordered
    cpp_dump((unordered_map<int, int>{{4, 6}, {2, 6}, {4, 3}}));

    unordered_multimap<char, int> unordered_multimap1;
    unordered_multimap1.emplace('c', 30);
    unordered_multimap1.emplace('a', 10);
    unordered_multimap1.emplace('b', 20);
    unordered_multimap1.emplace('a', 40);
    cpp_dump(unordered_multimap1);

    cpp_dump((unordered_set{3, 1, 4, 1, 5}));

    unordered_multiset<int> unordered_multiset1;
    unordered_multiset1.insert(3);
    unordered_multiset1.insert(1);
    unordered_multiset1.insert(4);
    unordered_multiset1.insert(1);
    cpp_dump(unordered_multiset1);
  }
}
