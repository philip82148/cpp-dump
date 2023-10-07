#include "../dump.hpp"

#include <bitset>
#include <complex>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

// These are often used to write code faster in competition programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using cpp_dump::dump;

struct class_a {
  int int_a   = 314159265;
  long long_b = 1;
  int get_a() const { return int_a; }
} class_a1;

struct class_b {
  static long static_long_a;
  int int_b        = 1;
  string str       = "This object has a pointer to itself.";
  class_b *pointer = this;
} class_b1;

long class_b::static_long_a = 358979;

enum class enum_a { s, k, l };

CPP_DUMP_DEFINE_EXPORT_ENUM(enum_a, enum_a::s, enum_a::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(decltype(class_a1), int_a, long_b, get_a());
CPP_DUMP_DEFINE_EXPORT_OBJECT(class_b, static_long_a, int_b, str, pointer);

int main(int argc, char *argv[]) {
  if (argc != 5) return 1;
  const char *filename = argv[1];
  int max_line_width   = stoi(argv[2]);
  int max_depth        = stoi(argv[3]);
  int es_index         = stoi(argv[4]);

  ofstream stream(filename);

  streambuf *clog_buf;
  clog_buf = clog.rdbuf(stream.rdbuf());

  CPP_DUMP_SET_OPTION(max_line_width, max_line_width);
  CPP_DUMP_SET_OPTION(max_depth, max_depth);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
  );

  // basic
  CPP_DUMP(false, 0, 0.0, '0', (const char *)"0", string{"0"}, string_view{"0"});
  CPP_DUMP(true, 3.14, 159265, "This is a test string");
  CPP_DUMP("This contains newline\nhere.", R"(This contains ".)", R"(This contains `.)");
  CPP_DUMP("\n\n\n\n");

  // vector & auto indent
  CPP_DUMP((vector<int>{3, 1, 4}));
  CPP_DUMP((vector<vector<int>>{{3}, {1, 4}, {1, 5, 9}}));
  CPP_DUMP((vector<vector<vector<int>>>{{}, {{2, 6}}, {{5}, {3}, {5}}}));
  const auto vec =
      (vector<vector<int>>{{2, 4, 6, 7, 8, 9, 0, 1, 1, 1, 7, 8, 9, 0, 1, 1, 1, 1, 1, 1, 1}});
  CPP_DUMP(
      (vector<int>{3, 1, 4}),
      vec,
      (vector<vector<vector<int>>>{{{2, 4, 6, 7, 8, 9, 0, 1, 1, 1, 3, 45}}})
  );
  CPP_DUMP(
      (vector<int>{2, 3}), (vector<int>{3, 4}), (vector<int>{2, 4, 4, 5, 6, 4, 5, 2, 4, 5, 3,
                                                             5, 6, 7, 5, 6, 6, 6, 7, 7, 8, 1})
  );
  CPP_DUMP(
      (vector<int>{2, 3}), (vector<int>{3, 4}), (vector<vector<int>>{{2}, {4}, {4}, {5}, {6}})
  );
  CPP_DUMP((vector{make_pair(1, 9), make_pair(3, 4), make_pair(5, 7)}));
  int c_style_array[10]{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
  CPP_DUMP(c_style_array);

  CPP_DUMP(vec);
  dump(vec);

  // pair&tuple
  CPP_DUMP(make_pair(8, 'a'));
  CPP_DUMP(make_tuple(
      9,
      "This is another string.",
      "This is another string.",
      "This is another string.",
      "This is another string.",
      7.9
  ));

  // map
  CPP_DUMP((map<double, string>{{3.2, "This is a pen."}, {3.8, "I have an apple."}}));
  CPP_DUMP((unordered_map<int, int>{{4, 6}, {2, 6}, {4, 3}}));

  // multimap
  multimap<char, int> multimap1;
  multimap1.emplace('c', 30);
  multimap1.emplace('a', 10);
  multimap1.emplace('b', 20);
  multimap1.emplace('a', 40);
  CPP_DUMP(multimap1);

  unordered_multimap<char, int> unordered_multimap1;
  unordered_multimap1.emplace('c', 30);
  unordered_multimap1.emplace('a', 10);
  unordered_multimap1.emplace('b', 20);
  unordered_multimap1.emplace('a', 40);
  CPP_DUMP(unordered_multimap1);

  // set
  CPP_DUMP((set{"A", "p", "p", "l", "e", " ", "P", "e", "n"}));
  CPP_DUMP((unordered_set{3, 1, 4, 1, 5}));

  // multiset
  multiset<int> multiset1;
  multiset1.insert(3);
  multiset1.insert(1);
  multiset1.insert(4);
  multiset1.insert(1);
  CPP_DUMP(multiset1);

  unordered_multiset<int> unordered_multiset1;
  unordered_multiset1.insert(3);
  unordered_multiset1.insert(1);
  unordered_multiset1.insert(4);
  unordered_multiset1.insert(1);
  CPP_DUMP(unordered_multiset1);

  // FIFO/LIFO
  queue<int> queue1;
  for (auto v : {1, 2, 3, 4, 5}) queue1.push(v);
  CPP_DUMP(queue1);

  priority_queue<char> priority_queue1;
  for (auto v : {'1', '2', '3', '4', '5'}) priority_queue1.push(v);
  CPP_DUMP(priority_queue1);

  stack<string> stack1;
  for (auto v : {"1", "2", "3", "4", "5"}) stack1.push(v);
  CPP_DUMP(stack1);

  // pointer
  int a = 65;
  int *int_ptr;
  int_ptr = &a;

  CPP_DUMP(int_ptr);
  CPP_DUMP(*int_ptr);

  const void *void_ptr = (void *)0x7ffd06586204;
  char *char_ptr       = (char *)&a;
  CPP_DUMP(void_ptr, char_ptr);
  CPP_DUMP(nullptr);
  shared_ptr<int> shared_ptr1(new int(42));
  weak_ptr<int> weak_ptr1 = shared_ptr1;
  unique_ptr<int> unique_ptr1(new int(42));
  auto c = ref(shared_ptr1);
  CPP_DUMP(shared_ptr1, weak_ptr1, unique_ptr1, c);
  shared_ptr1.reset(new int(3));
  CPP_DUMP(shared_ptr1, weak_ptr1, unique_ptr1);

  // object
  CPP_DUMP(class_a1);
  CPP_DUMP(class_b1);

  // enum
  enum_a enum_a_s = enum_a::s;
  enum_a enum_a_k = enum_a::k;
  enum_a enum_a_l = enum_a::l;
  CPP_DUMP(int_ptr, enum_a_s, enum_a_k, enum_a_l);

  // other
  bitset<10> bitset1(0xca);
  CPP_DUMP(bitset1);

  optional<int> optional = 1;
  CPP_DUMP(optional);
  optional = nullopt;
  CPP_DUMP(optional);
  CPP_DUMP(nullopt);

  variant<int, string> variant1 = "";
  CPP_DUMP(variant1);
  variant1 = 0;
  CPP_DUMP(variant1);

  complex complex1{0.0, -1.0};
  CPP_DUMP(complex1);
  complex complex2{10.0, 1.0};
  CPP_DUMP(complex2);

  logic_error logic_error1("This is a test error.");
  class original_error : public logic_error {
    using logic_error::logic_error;
  } original_error1("This is an original error.");
  CPP_DUMP(logic_error1, original_error1);

  // iterator
  CPP_DUMP(vec.begin());

  // unsupported type
  enum class unsupported { k, l } unsupported_enum = unsupported::k;
  CPP_DUMP(unsupported_enum);

  // extra
  CPP_DUMP(cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax, cpp_dump::es_value);

  // max_iteration_count
  array<int, 50> array50;
  map<int, int> map50;
  set<int> set50;
  rep(i, 50) {
    array50[i] = i;
    map50.emplace(i, i + 1);
    set50.emplace(i);
  }
  CPP_DUMP_SET_OPTION(max_iteration_count, 10);
  CPP_DUMP(array50, map50, set50);
  CPP_DUMP_SET_OPTION(max_iteration_count, 40);
  CPP_DUMP(array50, map50, set50);

  clog.rdbuf(clog_buf);
}
