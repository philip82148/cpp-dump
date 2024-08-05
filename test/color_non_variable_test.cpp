#include "../dump.hpp"
namespace cp = cpp_dump;

#include <array>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;

struct class_a {
  int int_a = 314159265;
  long long_b = 1;
  static long static_long_a;
  string a_str() const { return to_string(int_a); }
} class_a1;
long class_a::static_long_a = 358979;

struct non_copyable_and_non_movable_class {
  string str_member;
  non_copyable_and_non_movable_class *pointer{this};
  reference_wrapper<non_copyable_and_non_movable_class> ref{*this};

  non_copyable_and_non_movable_class(const non_copyable_and_non_movable_class &) = delete;
  non_copyable_and_non_movable_class &operator=(const non_copyable_and_non_movable_class &) =
      delete;

  non_copyable_and_non_movable_class(non_copyable_and_non_movable_class &&) = delete;
  non_copyable_and_non_movable_class &operator=(non_copyable_and_non_movable_class &&) = delete;

  non_copyable_and_non_movable_class() = delete;

  non_copyable_and_non_movable_class(const string &s) : str_member(s) {}
} non_copyable_and_non_movable_class1("This object has a pointer and reference_wrapper to itself.");

enum partially_supported_enum { supported, unsupported };
CPP_DUMP_DEFINE_EXPORT_ENUM(partially_supported_enum, supported);

int main(int argc, char *argv[]) {
  if (argc != 2) return 1;
  auto es_style_ = (array{
      cp::es_style_t::no_es,
      cp::es_style_t::original,
      cp::es_style_t::by_syntax,
  }[stoi(argv[1])]);
  bool detailed_es = es_style_ == cp::es_style_t::by_syntax;

  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  CPP_DUMP_SET_OPTION(es_style, es_style_);
  CPP_DUMP_SET_OPTION(detailed_class_es, detailed_es);
  CPP_DUMP_SET_OPTION(detailed_member_es, detailed_es);
  CPP_DUMP_SET_OPTION(detailed_number_es, detailed_es);

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
          ""                         // number_op: default
      })
  );

  // basic
  cpp_dump(false, 0, 0.0, '0', (const char *)"0", string{"0"}, string_view{"0"});
  cpp_dump(true, 3.14, 159265, "This is a test string");
  cpp_dump(-0, -3.14, -159265);
  cpp_dump("This contains newline\nhere.", R"(This contains ".)", R"(This contains `.)");

  // vector
  vector<vector<vector<int>>> vec{{}, {{2, 6}}, {{5}, {3}, {5}}};
  vector<int> vec2 = {1, 2, 3, 4, 5, 6, 7, 8};
  vector<int> vec3{3, 1};
  cpp_dump(vec);
  for (auto print : {false, true}) {
    CPP_DUMP_SET_OPTION(print_expr, print);
    cpp_dump(vec2);
    cpp_dump(vec3, vec3);
  }

  // pair&tuple
  pair<int, char> pair1(8, 'a');
  tuple<int, const char *, string_view, std::string, std::string, double> tuple1{
      9,
      "This is another string.",
      "This is another string.",
      "This is another string.",
      "This is another string.",
      7.9,
  };
  cpp_dump(pair1);
  cpp_dump(tuple1);

  // map
  map<double, string> map1{{3.2, "This is a pen."}, {3.8, "I have an apple."}};
  cpp_dump(map1);

  // multimap
  multimap<char, int> multimap1{
      {'c', 30},
      {'a', 10},
      {'b', 20},
      {'a', 40},
      {'c', 31},
      {'a', 12},
      {'b', 23},
      {'a', 44},
  };
  cpp_dump(multimap1);

  // set
  set<string> set1{"A", "p", "p", "l", "e", " ", "P", "e", "n"};
  cpp_dump(set1);

  // multiset
  multiset<int> multiset1;
  multiset1.insert(3);
  multiset1.insert(1);
  multiset1.insert(4);
  multiset1.insert(1);
  cpp_dump(multiset1);

  // FIFO/LIFO
  queue<int> queue1;
  for (auto v : {3, 2}) {
    cpp_dump(queue1);
    queue1.push(v);
  }
  cpp_dump(queue1);

  priority_queue<char> priority_queue1;
  for (auto v : {'3'}) {
    cpp_dump(priority_queue1);
    priority_queue1.push(v);
  }
  cpp_dump(priority_queue1);

  stack<string> stack1;
  for (auto v : {"3"}) {
    cpp_dump(stack1);
    stack1.push(v);
  }
  cpp_dump(stack1);

  // pointer
  int a = 65;
  int *int_ptr = &a;

  cpp_dump(int_ptr);
  cpp_dump(nullptr);

  // object
  cpp_dump(class_a1);
  cpp_dump(non_copyable_and_non_movable_class1);

  bitset<10> bitset10(0xca);
  cpp_dump(bitset10);

  optional<int> optional1 = 1;
  cpp_dump(optional1);
  cpp_dump(nullopt);

  variant<int, string> variant1 = "";
  cpp_dump(variant1);

  complex complex1{0.0, -1.0};
  cpp_dump(complex1);

  logic_error logic_error1("This is a test error.");
  cpp_dump(logic_error1);

  // iterator
  cpp_dump(vec.begin());
  CPP_DUMP_SET_OPTION(enable_asterisk, true);
  cpp_dump(vec.begin());
  CPP_DUMP_SET_OPTION(enable_asterisk, false);

  // user-defined enum
  cpp_dump(partially_supported_enum::supported);
  cpp_dump(partially_supported_enum::unsupported);

  // unsupported type
  struct unsupported_class {
    int k;
    string str() const { return ""; }
  } unsupported_class1;
  cpp_dump(unsupported_class1);

  // extra
  cpp_dump(cp::es_style_t::no_es);
  cpp_dump(cp::es_style_t::original);
  cpp_dump(cp::es_style_t::by_syntax);
  cpp_dump(cp::es_value);

  // manipulators
  cpp_dump(vec2 | cp::index());
  cpp_dump(vec2 | cp::front(3) | cp::index());
  cpp_dump(cp::es_value | cp::index());
  cpp_dump(cp::es_value | cp::index() | cp::front(1));

  CPP_DUMP_SET_OPTION(print_expr, false);
  cpp_dump(-1 | cp::bin(), -1 | cp::oct(), -1 | cp::hex(), -1 | cp::dec());
  cpp_dump(1 | cp::bin(), 1 | cp::oct(), 1 | cp::hex(), 1 | cp::dec());
  cpp_dump(-1 | cp::ubin(), -1 | cp::uoct(), -1 | cp::uhex(), -1 | cp::udec());
  cpp_dump(1 | cp::ubin(), 1 | cp::uoct(), 1 | cp::uhex(), 1 | cp::udec());

  // format
  CPP_DUMP_SET_OPTION(print_expr, true);
  cpp_dump(3.14159265f | cp::format("%+.3f"));
  cpp_dump(3.141592653589793L | cp::format("%+.15Lf"));
  cpp_dump(314159265 | cp::format("%+020d"));
  cpp_dump(31415926535ll | cp::format("%+020lld"));
  cpp_dump(complex1 | cp::format("%.10f"));
  cpp_dump(-3.14159265f | cp::format("[%10.3f]"));

  vector<vector<bool>> vec4 = {
      {true, false, true, false},
      {false, false, false, false},
      {true, true, true, true},
  };

  // bw, boolnum
  cpp_dump(vec4);
  cpp_dump(vec4 | cp::bw());
  cpp_dump(vec4 | cp::bw(true));
  cpp_dump(vec4 | cp::boolnum());

  // charhex
  std::string_view sv1 = "Pi=3.14...";
  vector<char> vec5(sv1.begin(), sv1.end());
  cpp_dump(vec5);
  cpp_dump(vec5 | cp::charhex());
}
