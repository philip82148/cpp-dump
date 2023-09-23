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

#define rep(i, n) for (int i = 0; i < (int)(n); i++)

using namespace std;
using cpp_dump::dump;

struct class_a {
  int a = 314159265;
  int b = 1;
  int get_a() const { return a; }
} object_a;

struct class_b {
  static int a;
  int b            = 1;
  string s         = "This object has a pointer to itself.";
  class_b *pointer = this;
} object_b;

int class_b::a = 358979;

enum class enum1 { s, k, l };

CPP_DUMP_DEFINE_EXPORT_ENUM(enum1, enum1::s, enum1::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(decltype(object_a), a, b, get_a());
CPP_DUMP_DEFINE_EXPORT_OBJECT(class_b, a, b, s, pointer);

int main() {
  ofstream stream{"./dump_test.txt"};

  streambuf *clog_buf;
  clog_buf = clog.rdbuf(stream.rdbuf());

  CPP_DUMP_SET_OPTION(max_line_width, 160);
  CPP_DUMP_SET_OPTION(max_depth, 5);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);

  // basic
  cpp_dump(false, 0, 0.0, '0', (const char *)"0", string{"0"}, string_view{"0"});
  cpp_dump(true, 3.14, 159265, "This is a test string");
  cpp_dump("This contains newline\nhere.", R"(This contains ".)", R"(This contains `.)");
  cpp_dump("\n\n\n\n");

  // vector & auto indent
  cpp_dump((vector<int>{3, 1, 4}));
  cpp_dump((vector<vector<int>>{{3}, {1, 4}, {1, 5, 9}}));
  cpp_dump((vector<vector<vector<int>>>{{}, {{2, 6}}, {{5}, {3}, {5}}}));
  const auto vec =
      (vector<vector<int>>{{2, 4, 6, 7, 8, 9, 0, 1, 1, 1, 7, 8, 9, 0, 1, 1, 1, 1, 1, 1, 1}});
  cpp_dump(
      (vector<int>{3, 1, 4}),
      vec,
      (vector<vector<vector<int>>>{{{2, 4, 6, 7, 8, 9, 0, 1, 1, 1, 3, 45}}})
  );
  cpp_dump(
      (vector<int>{2, 3}), (vector<int>{3, 4}), (vector<int>{2, 4, 4, 5, 6, 4, 5, 2, 4, 5, 3,
                                                             5, 6, 7, 5, 6, 6, 6, 7, 7, 8, 1})
  );
  cpp_dump(
      (vector<int>{2, 3}), (vector<int>{3, 4}), (vector<vector<int>>{{2}, {4}, {4}, {5}, {6}})
  );
  cpp_dump((vector{make_pair(1, 9), make_pair(3, 4), make_pair(5, 7)}));
  int c_style_array[10]{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
  cpp_dump(c_style_array);

  cpp_dump(vec);
  dump(vec);

  // pair&tuple
  cpp_dump(make_pair(8, 'a'));
  cpp_dump(make_tuple(
      9,
      "This is another string.",
      "This is another string.",
      "This is another string.",
      "This is another string.",
      7.9
  ));

  // map
  cpp_dump((map<double, string>{{3.2, "This is a pen."}, {3.8, "I have an apple."}}));
  cpp_dump((unordered_map<int, int>{{4, 6}, {2, 6}, {4, 3}}));

  // multimap
  multimap<char, int> multimap1;
  multimap1.emplace('c', 30);
  multimap1.emplace('a', 10);
  multimap1.emplace('b', 20);
  multimap1.emplace('a', 40);
  cpp_dump(multimap1);

  unordered_multimap<char, int> unordered_multimap1;
  unordered_multimap1.emplace('c', 30);
  unordered_multimap1.emplace('a', 10);
  unordered_multimap1.emplace('b', 20);
  unordered_multimap1.emplace('a', 40);
  cpp_dump(unordered_multimap1);

  // set
  cpp_dump((set{"A", "p", "p", "l", "e", " ", "P", "e", "n"}));
  cpp_dump((unordered_set{3, 1, 4, 1, 5}));

  // multiset
  multiset<int> multiset1;
  multiset1.insert(3);
  multiset1.insert(1);
  multiset1.insert(4);
  multiset1.insert(1);
  cpp_dump(multiset1);

  unordered_multiset<int> unordered_multiset1;
  unordered_multiset1.insert(3);
  unordered_multiset1.insert(1);
  unordered_multiset1.insert(4);
  unordered_multiset1.insert(1);
  cpp_dump(unordered_multiset1);

  // FIFO/LIFO
  queue<int> queue1;
  for (auto v : {1, 2, 3, 4, 5}) queue1.push(v);
  cpp_dump(queue1);

  priority_queue<char> priority_queue1;
  for (auto v : {'1', '2', '3', '4', '5'}) priority_queue1.push(v);
  cpp_dump(priority_queue1);

  stack<string> stack1;
  for (auto v : {"1", "2", "3", "4", "5"}) stack1.push(v);
  cpp_dump(stack1);

  enum class s { k } b = s::k;
  cpp_dump(b);

  // pointer
  int a = 65;
  int *int_p;
  int_p = &a;

  cpp_dump(int_p);
  cpp_dump(*int_p);

  const void *void_p = (void *)0x7ffd06586204;
  char *char_p       = (char *)&a;
  cpp_dump(void_p, char_p);
  cpp_dump(nullptr);
  shared_ptr<int> shared_ptr1(new int(42));
  weak_ptr<int> weak_ptr1 = shared_ptr1;
  unique_ptr<int> unique_ptr1(new int(42));
  auto c = ref(shared_ptr1);
  cpp_dump(shared_ptr1, weak_ptr1, unique_ptr1, c);
  shared_ptr1.reset(new int(3));
  cpp_dump(shared_ptr1, weak_ptr1, unique_ptr1);

  // object
  cpp_dump(object_a);
  cpp_dump(object_b);

  // enum
  enum1 s = enum1::s;
  enum1 k = enum1::k;
  enum1 l = enum1::l;
  cpp_dump(int_p, s, k, l);

  // other
  bitset<10> bitset1(0xca);
  cpp_dump(bitset1);

  optional<int> optional = 1;
  cpp_dump(optional);
  optional = nullopt;
  cpp_dump(optional);
  cpp_dump(nullopt);

  variant<int, string> variant1 = "";
  cpp_dump(variant1);
  variant1 = 0;
  cpp_dump(variant1);

  complex complex1{0.0, -1.0};
  cpp_dump(complex1);
  complex complex2{10.0, 1.0};
  cpp_dump(complex2);

  logic_error logic_error1("This is a test error.");
  class original_error : public logic_error {
    using logic_error::logic_error;
  } original_error1("This is an original error.");
  cpp_dump(logic_error1, original_error1);

  // iterator
  cpp_dump(vec.begin());

  // unsupported type
  enum class unsupported { k, l } unsupported_enum = unsupported::k;
  cpp_dump(unsupported_enum);

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
  cpp_dump(array50, map50, set50);
  CPP_DUMP_SET_OPTION(max_iteration_count, 40);
  cpp_dump(array50, map50, set50);

  clog.rdbuf(clog_buf);
}
