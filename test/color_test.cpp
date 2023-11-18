#include "../dump.hpp"
namespace cp = cpp_dump;
using cp::dump;

#include <exception>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
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

int main(int argc, char *argv[]) {
  if (argc != 3) return 1;
  int max_line_width_ = stoi(argv[1]);
  int max_depth_ = stoi(argv[2]);

  CPP_DUMP_SET_OPTION(max_line_width, max_line_width_);
  CPP_DUMP_SET_OPTION(max_depth, max_depth_);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);

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

  // basic
  cpp_dump(false, 0, 0.0, '0', (const char *)"0", string{"0"}, string_view{"0"});
  cpp_dump(true, 3.14, 159265, "This is a test string");
  cpp_dump("This contains newline\nhere.", R"(This contains ".)", R"(This contains `.)");

  // vector
  auto vec = vector<vector<vector<int>>>{{}, {{2, 6}}, {{5}, {3}, {5}}};
  cpp_dump(vec);
  dump(vec);

  // pair&tuple
  auto pair1 = make_pair(8, 'a');
  auto tuple1 = make_tuple(
      9,
      "This is another string.",
      "This is another string.",
      "This is another string.",
      "This is another string.",
      7.9
  );
  cpp_dump(pair1);
  cpp_dump(tuple1);

  // map
  auto map1 = map<double, string>{{3.2, "This is a pen."}, {3.8, "I have an apple."}};
  cpp_dump(map1);

  // multimap
  multimap<char, int> multimap1;
  multimap1.emplace('c', 30);
  multimap1.emplace('a', 10);
  multimap1.emplace('b', 20);
  multimap1.emplace('a', 40);
  multimap1.emplace('c', 31);
  multimap1.emplace('a', 12);
  multimap1.emplace('b', 23);
  multimap1.emplace('a', 44);
  cpp_dump(multimap1);

  // set
  auto set1 = set<string>{"A", "p", "p", "l", "e", " ", "P", "e", "n"};
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
  int *int_ptr;
  int_ptr = &a;

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

  // unsupported type
  struct unsupported_class {
    int k;
    string str() const { return ""; }
  } unsupported_class1;
  cpp_dump(unsupported_class1);

  // extra
  cpp_dump(cpp_dump::es_style_t::no_es);
  cpp_dump(cpp_dump::es_style_t::by_syntax);
  cpp_dump(cpp_dump::es_value);

  // manipulators
  vector<int> vec2 = {1, 2, 3, 4, 5, 6, 7, 8};

  cpp_dump(vec2 | cp::cont_index());
  cpp_dump(vec2 | cp::show_front(3) | cp::cont_index());
  cpp_dump(cpp_dump::es_value | cp::cont_index());
}
