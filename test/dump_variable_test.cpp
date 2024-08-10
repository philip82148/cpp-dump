#include <array>
#include <memory>
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

namespace ns {

template <typename>
struct template_class {
  int member_var = 5;
  std::string member_func() const { return "This is a member_func."; }
};

}  // namespace ns

int main(int argc, char *argv[]) {
  if (argc != 4) return 1;
  bool compiler_dependent = static_cast<bool>(stoi(argv[1]));
  auto es_style_ = (array{
      cp::types::es_style_t::no_es,
      cp::types::es_style_t::original,
      cp::types::es_style_t::by_syntax,
  }[stoi(argv[2])]);
  bool detailed_es = es_style_ == cp::types::es_style_t::by_syntax;
  bool color_test = stoi(argv[3]);

  CPP_DUMP_SET_OPTION(es_style, es_style_);
  CPP_DUMP_SET_OPTION(detailed_class_es, detailed_es);
  CPP_DUMP_SET_OPTION(detailed_member_es, detailed_es);
  CPP_DUMP_SET_OPTION(detailed_number_es, detailed_es);

  if (color_test)
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
    cpp_dump(ns::template_class<ns::template_class<int>>());
    cpp_dump(original_class());
    cpp_dump(unsupported_original_class());
  } else {
    // pointer
    const void *void_ptr = (void *)0x7ffd06586204;
    cpp_dump(void_ptr);

    // addr
    const int *int_ptr = reinterpret_cast<const int *>(void_ptr);
    const int **int_ptr_ptr = &int_ptr;
    unique_ptr<const int **> int_ptr_ptr_ptr(new const int **(int_ptr_ptr));
    cpp_dump(int_ptr | cp::addr());
    cpp_dump(int_ptr_ptr | cp::addr(1));
    cpp_dump(int_ptr_ptr_ptr | cp::addr(2));

    // unordered
    cpp_dump((unordered_map<int, int>{{4, 6}, {2, 6}, {4, 3}}));

    unordered_multimap<char, int> unordered_multimap1{
        {'c', 30},
        {'a', 10},
        {'b', 20},
        {'a', 40},
    };
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
