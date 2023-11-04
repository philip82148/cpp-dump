#include <array>
#include <bitset>
#include <complex>
#include <exception>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>
//
#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using namespace cpp_dump;

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

struct non_copyable_and_non_movable_class_iterator_end {};

struct non_copyable_and_non_movable_class_iterator {
  int index = 0;
  auto operator*() const {
    // rvalue
    return non_copyable_and_non_movable_class{"This is non_copyable_and_non_movable_class."};
  }
  bool operator!=(const non_copyable_and_non_movable_class_iterator_end &) const {
    return index < 2;
  }
  non_copyable_and_non_movable_class_iterator &operator++() {
    ++index;
    return *this;
  }
};

struct non_copyable_and_non_movable_class_container {
  auto begin() const { return non_copyable_and_non_movable_class_iterator(); }
  auto end() const { return non_copyable_and_non_movable_class_iterator_end(); }
} non_copyable_and_non_movable_class_container1;

enum class enum_a { s, k, l };

CPP_DUMP_DEFINE_EXPORT_ENUM(enum_a, enum_a::s, enum_a::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(decltype(class_a1), int_a, long_b, a_str());
CPP_DUMP_DEFINE_EXPORT_OBJECT(non_copyable_and_non_movable_class, str_member, pointer, ref);

int main(int argc, char *argv[]) {
  if (argc != 4) return 1;
  int max_line_width_ = stoi(argv[1]);
  int max_depth_ = stoi(argv[2]);
  int es_index = stoi(argv[3]);

  CPP_DUMP_SET_OPTION(max_line_width, max_line_width_);
  CPP_DUMP_SET_OPTION(max_depth, max_depth_);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
  );

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
  int c_style_array[40]{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 3, 1, 4, 1, 5, 9, 2, 6, 5, 3,
                        3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
  cpp_dump(c_style_array);

  cpp_dump(vec);
  dump(vec);

  dump(c_style_array, c_style_array);

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
  cpp_dump((set<string>{"A", "p", "p", "l", "e", " ", "P", "e", "n"}));

  // multiset
  multiset<int> multiset1;
  multiset1.insert(3);
  multiset1.insert(1);
  multiset1.insert(4);
  multiset1.insert(1);
  cpp_dump(multiset1);

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

  // pointer
  int a = 65;
  int *int_ptr;
  int_ptr = &a;

  cpp_dump(int_ptr);
  cpp_dump(*int_ptr);

  char *char_ptr = (char *)&a;
  cpp_dump(char_ptr);
  cpp_dump(nullptr);
  shared_ptr<int> shared_ptr1(new int(42));
  weak_ptr<int> weak_ptr1 = shared_ptr1;
  unique_ptr<int> unique_ptr1(new int(42));
  auto c = ref(shared_ptr1);
  cpp_dump(shared_ptr1, weak_ptr1, unique_ptr1, c);
  shared_ptr1.reset(new int(3));
  cpp_dump(shared_ptr1, weak_ptr1, unique_ptr1);

  // object
  cpp_dump(class_a1);
  cpp_dump(non_copyable_and_non_movable_class1);

  // enum
  enum_a enum_a_s = enum_a::s;
  enum_a enum_a_k = enum_a::k;
  enum_a enum_a_l = enum_a::l;
  cpp_dump(int_ptr, enum_a_s, enum_a_k, enum_a_l);

  // other
  bitset<2> bitset2(0x1);
  bitset<3> bitset3(0x2);
  bitset<4> bitset4(0x3);
  bitset<5> bitset5(0x4);
  bitset<7> bitset7(0x3a);
  bitset<8> bitset8(0x3a);
  bitset<9> bitset9(0xca);
  bitset<10> bitset10(0xca);
  cpp_dump(bitset2, bitset3, bitset4, bitset5, bitset7, bitset8, bitset9, bitset10);

  optional<int> optional1 = 1;
  cpp_dump(optional1);
  optional1 = nullopt;
  cpp_dump(optional1);
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

  // extra
  cpp_dump(cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax, cpp_dump::es_value);

  // int_style()
  cpp_dump(
      int_style(16) << 10000u,
      int_style(16) << 10000l,
      int_style(16) << 10000ul,
      int_style(16) << 10000ll,
      int_style(16) << 10000ull
  );

  // show_*()
  array<int, 50> array50;
  map<int, int> map50;
  set<int> set50;
  rep(i, 50) {
    array50[i] = i;
    map50.emplace(i, i + 1);
    set50.emplace(i);
  }

  CPP_DUMP_SET_OPTION(max_iteration_count, 2);
  cpp_dump(array50, map50, set50);
  cpp_dump(show_front() << array50, show_front() << map50, show_front() << set50);
  cpp_dump(show_middle() << array50, show_middle() << map50, show_middle() << set50);
  cpp_dump(show_back() << array50, show_back() << map50, show_back() << set50);
  cpp_dump(show_both_ends() << array50, show_both_ends() << map50, show_both_ends() << set50);

  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  cpp_dump(show_front(10) << array50, show_front(10) << map50, show_front(10) << set50);
  cpp_dump(show_back(10) << array50, show_back(10) << map50, show_back(10) << set50);
  cpp_dump(show_middle(10) << array50, show_middle(10) << map50, show_middle(10) << set50);
  cpp_dump(show_both_ends(10) << array50, show_both_ends(10) << map50, show_both_ends(10) << set50);

  cpp_dump(show_front(40) << array50, show_front(40) << map50, show_front(40) << set50);
  cpp_dump(show_middle(40) << array50, show_middle(40) << map50, show_middle(40) << set50);
  cpp_dump(show_back(40) << array50, show_back(40) << map50, show_back(40) << set50);
  cpp_dump(show_both_ends(40) << array50, show_both_ends(40) << map50, show_both_ends(40) << set50);

  auto vec2 = vector<vector<vector<int>>>{
      {{1}},
      {{2, 6}},
      {
          {5, 6, 7},
          {3, 6, 8, 9},
          {5, 7, 8, 10, 11},
      }};

  cpp_dump(vec2, show_back(2) << show_both_ends(1) << show_back(2) << vec2);
  cpp_dump(vec2, show_both_ends(2) << show_both_ends(2) << show_both_ends(2) << vec2);

  cpp_dump(multimap1, show_both_ends(2) << show_both_ends(2) << multimap1);
  cpp_dump(multiset1, show_middle(1) << show_back(1) << multiset1);

  auto multimap2 = multimap<vector<int>, vector<int>>{
      {{1, 2, 4, 2}, {11, 22, 11, 22}},
      {{1, 2, 4, 2}, {14, 21, 11, 22}},
      {{3, 4, 0}, {21, 5, 2, 3 - 8}},
      {{5, 6, 7, 8}, {14, -2}},
      {{1, 2, 4, 2}, {0, 5, 25}},
  };
  cpp_dump(show_front(2) << map_key_and_value(show_back(2), show_front(2)) << multimap2);

  cpp_dump(
      int_style(12, 3, 3) << show_front(2) << int_style(2, 0, 0)
                          << map_key_and_value(int_style(16, 3, 3) << show_back(2), show_front(2))
                          << multimap2
  );
  cpp_dump(
      int_style(12, 3, 3) << int_style(8, 3, 3)
                          << map_key_and_value(int_style(16, 3, 3) << show_back(2), show_front(2))
                          << multimap2
  );
  cpp_dump(
      show_front(1) << map_value(
          int_style(8, 3, 3) << show_middle(1) << int_style(16, 3, 3) << show_middle(2)
                             << int_style(5, 3, 3)
      ) << multimap2
  );
  rep(i, 17) cpp_dump(i, int_style(i) << 0x10000);
  vector<vector<int>> vec3{{1, 20, 4}, {-2, 0, -10}};
  cpp_dump(vec3, int_style(10, 2, 0, false, true) << vec3, int_style10(2) << vec3);
  dump(int_style(10, 8, 4, false, false /**/) << 1);
  dump(int_style(10, 8, 4, false, true /* */) << 2);
  dump(int_style(10, 8, 4, false, true /**/) << -3);
  dump(int_style(10, 8, 4, true, false /**/) << 1);
  dump(int_style(10, 8, 4, true, true /* */) << 2);
  dump(int_style(10, 8, 4, true, true /**/) << -3);

  dump(int_style(10, 9, 4, false, false /**/) << 4);
  dump(int_style(10, 9, 4, false, true /* */) << 5);
  dump(int_style(10, 9, 4, false, true /**/) << -6);
  dump(int_style(10, 9, 4, true, false /**/) << 4);
  dump(int_style(10, 9, 4, true, true /* */) << 5);
  dump(int_style(10, 9, 4, true, true /**/) << -6);

  dump(int_style(10, 7, 4, false, false /**/) << 7);
  dump(int_style(10, 7, 4, false, true /* */) << 8);
  dump(int_style(10, 7, 4, false, true /**/) << -9);
  dump(int_style(10, 7, 4, true, false /**/) << 7);
  dump(int_style(10, 7, 4, true, true /* */) << 8);
  dump(int_style(10, 7, 4, true, true /**/) << -9);

  dump(int_style(10, 1, 4, false, false /**/) << 1);
  dump(int_style(10, 1, 4, false, true /* */) << 2);
  dump(int_style(10, 1, 4, false, true /**/) << -3);
  dump(int_style(10, 1, 4, true, false /**/) << 1);
  dump(int_style(10, 1, 4, true, true /* */) << 2);
  dump(int_style(10, 1, 4, true, true /**/) << -3);

  dump(int_style(10, 3, 4, false, false /**/) << 4);
  dump(int_style(10, 3, 4, false, true /* */) << 5);
  dump(int_style(10, 3, 4, false, true /**/) << -6);
  dump(int_style(10, 3, 4, true, false /**/) << 4);
  dump(int_style(10, 3, 4, true, true /* */) << 5);
  dump(int_style(10, 3, 4, true, true /**/) << -6);

  dump(int_style(10, 4, 4, false, false /**/) << 7);
  dump(int_style(10, 4, 4, false, true /* */) << 8);
  dump(int_style(10, 4, 4, false, true /**/) << -9);
  dump(int_style(10, 4, 4, true, false /**/) << 7);
  dump(int_style(10, 4, 4, true, true /* */) << 8);
  dump(int_style(10, 4, 4, true, true /**/) << -9);

  dump(int_style(10, 5, 4, false, false /**/) << 0);
  dump(int_style(10, 5, 4, false, true /* */) << 1);
  dump(int_style(10, 5, 4, false, true /**/) << -2);
  dump(int_style(10, 5, 4, true, false /**/) << 0);
  dump(int_style(10, 5, 4, true, true /* */) << 1);
  dump(int_style(10, 5, 4, true, true /**/) << -2);

  dump(int_style(10, 5, 4, true, false /**/) << 10000);
  dump(int_style(10, 5, 4, true, true /* */) << 20000);
  dump(int_style(10, 5, 4, true, true /**/) << -30000);
  dump(int_style(10, 5, 4, true, false /**/) << 4000);
  dump(int_style(10, 5, 4, true, true /* */) << 5000);
  dump(int_style(10, 5, 4, true, true /**/) << -6000);
  dump(int_style(10, 5, 4, true, false /**/) << 700);
  dump(int_style(10, 5, 4, true, true /* */) << 800);
  dump(int_style(10, 5, 4, true, true /**/) << -900);
  dump(int_style(10, 2, 4, true, false /**/) << 10000);
  dump(int_style(10, 2, 4, true, true /* */) << 20000);
  dump(int_style(10, 2, 4, true, true /**/) << -30000);
  dump(int_style(10, 2, 4, true, false /**/) << 4000);
  dump(int_style(10, 2, 4, true, true /* */) << 5000);
  dump(int_style(10, 2, 4, true, true /**/) << -6000);
  dump(int_style(10, 2, 4, true, false /**/) << 700);
  dump(int_style(10, 2, 4, true, true /* */) << 800);
  dump(int_style(10, 2, 4, true, true /**/) << -900);
  dump(int_style(10, 2, 4, true, false /**/) << 10);
  dump(int_style(10, 2, 4, true, true /* */) << 20);
  dump(int_style(10, 2, 4, true, true /**/) << -30);
  dump(int_style(10, 2, 4, true, false /**/) << 4);
  dump(int_style(10, 2, 4, true, true /* */) << 5);
  dump(int_style(10, 2, 4, true, true /**/) << -6);

  dump(int_style(16, 1, 2, true, false) << 0x83);
  dump(int_style(16, 1, 2, true, true) << 0x84);
  dump(int_style(16, 1, 2, true, true) << -0x74);
  dump(int_style(16, 1, 2, false, true) << -0x85);

  dump(int_style(16, 2, 2, true, false) << 0x86);
  dump(int_style(16, 2, 2, true, true) << 0x87);
  dump(int_style(16, 2, 2, true, true) << -0x77);
  dump(int_style(16, 2, 2, false, true) << 0x88);

  dump(int_style(16, 3, 2, true, false) << 0x89);
  dump(int_style(16, 3, 2, true, true) << 0x8a);
  dump(int_style(16, 3, 2, true, true) << -0x7a);
  dump(int_style(16, 3, 2, false, true) << 0x8b);

  dump(int_style(16, 4, 2, true, false) << 0x8c);
  dump(int_style(16, 4, 2, true, true) << 0x8d);
  dump(int_style(16, 4, 2, true, true) << -0x7d);
  dump(int_style(16, 4, 2, false, true) << 0x8e);

  dump(int_style(16, 5, 2, true, false) << 0x8f);
  dump(int_style(16, 5, 2, true, true) << 0x80);
  dump(int_style(16, 5, 2, true, true) << -0x71);
  dump(int_style(16, 5, 2, false, true) << 0x82);

  dump(int_style(10, 5, 4, false, false /**/) << 0);
  dump(int_style(10, 5, 4, true, false /**/) << 0);
  dump(int_style(16, 5, 4, false, false /**/) << 0);
  dump(int_style(16, 5, 4, true, false /**/) << 0);
  dump(int_style(10, 5, 4, false, true /* */) << 0);
  dump(int_style(10, 5, 4, true, true /* */) << 0);
  dump(int_style(16, 5, 4, false, true /* */) << 0);
  dump(int_style(16, 5, 4, true, true /* */) << 0);

  dump(int_style(10, 5, 0, false, false /**/) << 0);
  dump(int_style(10, 5, 0, true, false /**/) << 0);
  dump(int_style(16, 5, 0, false, false /**/) << 0);
  dump(int_style(16, 5, 0, true, false /**/) << 0);
  dump(int_style(10, 5, 0, false, true /* */) << 0);
  dump(int_style(10, 5, 0, true, true /* */) << 0);
  dump(int_style(16, 5, 0, false, true /* */) << 0);
  dump(int_style(16, 5, 0, true, true /* */) << 0);

  CPP_DUMP_SET_OPTION(max_depth, 2);
  cpp_dump(
      show_front(2) << show_middle(1) << show_back(2) << show_both_ends(1)
                    << non_copyable_and_non_movable_class1
  );
  cpp_dump(non_copyable_and_non_movable_class_container1);
  cpp_dump(show_front(1) << non_copyable_and_non_movable_class_container1);
}
