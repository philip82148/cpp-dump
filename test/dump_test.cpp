#include "../dump.hpp"
namespace mn = cpp_dump::mn;
using cpp_dump::dump;

#ifdef USE_BITS_STDC
// This is a test for competitive programming.
#include <bits/stdc++.h>
#pragma message("<bits/stdc++.h> is used.")
#else
#include <array>
#include <bitset>
#include <complex>
#include <exception>
#include <iomanip>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>
#endif

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

struct ostream_able_class_a {
} ostream_able_class_a_1;

ostream &operator<<(ostream &os, const ostream_able_class_a &) {
  os << "ostream_able_class_a";
  return os;
}

// support non copyable iterator
// https://stackoverflow.com/questions/2568294/is-it-a-good-idea-to-create-an-stl-iterator-which-is-noncopyable
struct non_copyable_non_const_iterator {
  int index = 0;

  non_copyable_non_const_iterator(non_copyable_non_const_iterator &&) = default;
  non_copyable_non_const_iterator &operator=(non_copyable_non_const_iterator &&) = default;

  non_copyable_non_const_iterator(const non_copyable_non_const_iterator &) = delete;
  non_copyable_non_const_iterator &operator=(const non_copyable_non_const_iterator &) = delete;

  non_copyable_non_const_iterator() = default;

  auto operator*() { return "This iterator is non-copyable and doesn't support const."; }
  bool operator!=(const non_copyable_non_const_iterator &) { return index < 2; }
  auto &operator++() {
    ++index;
    return *this;
  }
};

struct container_of_non_copyable_non_const_iterator {
  auto begin() const { return non_copyable_non_const_iterator(); }
  auto end() const { return non_copyable_non_const_iterator(); }
} container_of_non_copyable_non_const_iterator1;

struct supported_iterator {
  auto operator*() const { return "This must not be printed since the container is unsupported."; }
  bool operator!=(const supported_iterator &) const { return true; }
  auto &operator++() { return *this; }
};

struct unsupported_container_of_supported_container_a {
  auto begin() { return supported_iterator(); }
  auto end() const { return supported_iterator(); }
} unsupported_container_of_supported_container_a1;

struct unsupported_container_of_supported_container_b {
  auto begin() const { return supported_iterator(); }
  auto end() { return supported_iterator(); }
} unsupported_container_of_supported_container_b1;

struct unsupported_non_const_class {
  auto operator*() { return "This must not be printed."; }
} unsupported_non_const_class1;

ostream &operator<<(ostream &os, unsupported_non_const_class &) {
  os << "This must not be printed.";
  return os;
}

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
  cpp_dump(logic_error1);

  // iterator
  cpp_dump(vec.begin());

  // ostream
  cpp_dump(ostream_able_class_a_1);

  // unsupported type
  struct unsupported_class {
    int k;
    string str() const { return ""; }
  } unsupported_class1;
  cpp_dump(unsupported_class1);

  // unsupported type (function, function&member pointers)
  cpp_dump(main, &main, &unsupported_class::k, &unsupported_class::str);

  // unsupported type (manipulators)
  cpp_dump(setw(5), boolalpha);

  // extra
  cpp_dump(cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax, cpp_dump::es_value);

  // mn::int_style()
  cpp_dump(
      mn::int_style(16) << 10000u,
      mn::int_style(16) << 10000l,
      mn::int_style(16) << 10000ul,
      mn::int_style(16) << 10000ll,
      mn::int_style(16) << 10000ull
  );

  // mn::show_*()
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
  cpp_dump(mn::front() << array50, mn::front() << map50, mn::front() << set50);
  cpp_dump(mn::middle() << array50, mn::middle() << map50, mn::middle() << set50);
  cpp_dump(mn::back() << array50, mn::back() << map50, mn::back() << set50);
  cpp_dump(mn::both_ends() << array50, mn::both_ends() << map50, mn::both_ends() << set50);

  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  cpp_dump(mn::front(10) << array50, mn::front(10) << map50, mn::front(10) << set50);
  cpp_dump(mn::back(10) << array50, mn::back(10) << map50, mn::back(10) << set50);
  cpp_dump(mn::middle(10) << array50, mn::middle(10) << map50, mn::middle(10) << set50);
  cpp_dump(mn::both_ends(10) << array50, mn::both_ends(10) << map50, mn::both_ends(10) << set50);

  cpp_dump(mn::front(40) << array50, mn::front(40) << map50, mn::front(40) << set50);
  cpp_dump(mn::middle(40) << array50, mn::middle(40) << map50, mn::middle(40) << set50);
  cpp_dump(mn::back(40) << array50, mn::back(40) << map50, mn::back(40) << set50);
  cpp_dump(mn::both_ends(40) << array50, mn::both_ends(40) << map50, mn::both_ends(40) << set50);

  auto vec2 = vector<vector<vector<int>>>{
      {{1}},
      {{2, 6}},
      {
          {5, 6, 7},
          {3, 6, 8, 9},
          {5, 7, 8, 10, 11},
      }};

  cpp_dump(vec2, mn::back(2) << mn::both_ends(1) << mn::back(2) << vec2);
  cpp_dump(vec2, mn::both_ends(2) << mn::both_ends(2) << mn::both_ends(2) << vec2);

  cpp_dump(multimap1, mn::both_ends(2) << mn::both_ends(2) << multimap1);
  cpp_dump(multiset1, mn::middle(1) << mn::back(1) << multiset1);

  auto multimap2 = multimap<vector<int>, vector<int>>{
      {{1, 2, 4, 2}, {11, 22, 11, 22}},
      {{1, 2, 4, 2}, {14, 21, 11, 22}},
      {{3, 4, 0}, {21, 5, 2, 3 - 8}},
      {{5, 6, 7, 8}, {14, -2}},
      {{1, 2, 4, 2}, {0, 5, 25}},
  };
  cpp_dump(mn::front(2) << mn::map_kv(mn::back(2), mn::front(2)) << multimap2);

  cpp_dump(
      mn::int_style(12, 3, 3) << mn::front(2) << mn::int_style(2, 0, 0)
                              << mn::map_kv(mn::int_style(16, 3, 3) << mn::back(2), mn::front(2))
                              << multimap2
  );
  cpp_dump(
      mn::int_style(12, 3, 3) << mn::int_style(8, 3, 3)
                              << mn::map_kv(mn::int_style(16, 3, 3) << mn::back(2), mn::front(2))
                              << multimap2
  );
  cpp_dump(
      mn::front(1) << mn::map_v(
          mn::int_style(8, 3, 3) << mn::middle(1) << mn::int_style(16, 3, 3) << mn::middle(2)
                                 << mn::int_style(5, 3, 3)
      ) << multimap2
  );
  rep(i, 17) cpp_dump(i, mn::int_style(i) << 0x10000);
  vector<vector<int>> vec3{{1, 20, 4}, {-2, 0, -10}};
  cpp_dump(vec3, mn::int_style(10, 2, 0, false, true) << vec3, mn::int_style10(2) << vec3);
  dump(mn::int_style(10, 8, 4, false, false /**/) << 1);
  dump(mn::int_style(10, 8, 4, false, true /* */) << 2);
  dump(mn::int_style(10, 8, 4, false, true /**/) << -3);
  dump(mn::int_style(10, 8, 4, true, false /**/) << 1);
  dump(mn::int_style(10, 8, 4, true, true /* */) << 2);
  dump(mn::int_style(10, 8, 4, true, true /**/) << -3);

  dump(mn::int_style(10, 9, 4, false, false /**/) << 4);
  dump(mn::int_style(10, 9, 4, false, true /* */) << 5);
  dump(mn::int_style(10, 9, 4, false, true /**/) << -6);
  dump(mn::int_style(10, 9, 4, true, false /**/) << 4);
  dump(mn::int_style(10, 9, 4, true, true /* */) << 5);
  dump(mn::int_style(10, 9, 4, true, true /**/) << -6);

  dump(mn::int_style(10, 7, 4, false, false /**/) << 7);
  dump(mn::int_style(10, 7, 4, false, true /* */) << 8);
  dump(mn::int_style(10, 7, 4, false, true /**/) << -9);
  dump(mn::int_style(10, 7, 4, true, false /**/) << 7);
  dump(mn::int_style(10, 7, 4, true, true /* */) << 8);
  dump(mn::int_style(10, 7, 4, true, true /**/) << -9);

  dump(mn::int_style(10, 1, 4, false, false /**/) << 1);
  dump(mn::int_style(10, 1, 4, false, true /* */) << 2);
  dump(mn::int_style(10, 1, 4, false, true /**/) << -3);
  dump(mn::int_style(10, 1, 4, true, false /**/) << 1);
  dump(mn::int_style(10, 1, 4, true, true /* */) << 2);
  dump(mn::int_style(10, 1, 4, true, true /**/) << -3);

  dump(mn::int_style(10, 3, 4, false, false /**/) << 4);
  dump(mn::int_style(10, 3, 4, false, true /* */) << 5);
  dump(mn::int_style(10, 3, 4, false, true /**/) << -6);
  dump(mn::int_style(10, 3, 4, true, false /**/) << 4);
  dump(mn::int_style(10, 3, 4, true, true /* */) << 5);
  dump(mn::int_style(10, 3, 4, true, true /**/) << -6);

  dump(mn::int_style(10, 4, 4, false, false /**/) << 7);
  dump(mn::int_style(10, 4, 4, false, true /* */) << 8);
  dump(mn::int_style(10, 4, 4, false, true /**/) << -9);
  dump(mn::int_style(10, 4, 4, true, false /**/) << 7);
  dump(mn::int_style(10, 4, 4, true, true /* */) << 8);
  dump(mn::int_style(10, 4, 4, true, true /**/) << -9);

  dump(mn::int_style(10, 5, 4, false, false /**/) << 0);
  dump(mn::int_style(10, 5, 4, false, true /* */) << 1);
  dump(mn::int_style(10, 5, 4, false, true /**/) << -2);
  dump(mn::int_style(10, 5, 4, true, false /**/) << 0);
  dump(mn::int_style(10, 5, 4, true, true /* */) << 1);
  dump(mn::int_style(10, 5, 4, true, true /**/) << -2);

  dump(mn::int_style(10, 5, 4, true, false /**/) << 10000);
  dump(mn::int_style(10, 5, 4, true, true /* */) << 20000);
  dump(mn::int_style(10, 5, 4, true, true /**/) << -30000);
  dump(mn::int_style(10, 5, 4, true, false /**/) << 4000);
  dump(mn::int_style(10, 5, 4, true, true /* */) << 5000);
  dump(mn::int_style(10, 5, 4, true, true /**/) << -6000);
  dump(mn::int_style(10, 5, 4, true, false /**/) << 700);
  dump(mn::int_style(10, 5, 4, true, true /* */) << 800);
  dump(mn::int_style(10, 5, 4, true, true /**/) << -900);
  dump(mn::int_style(10, 2, 4, true, false /**/) << 10000);
  dump(mn::int_style(10, 2, 4, true, true /* */) << 20000);
  dump(mn::int_style(10, 2, 4, true, true /**/) << -30000);
  dump(mn::int_style(10, 2, 4, true, false /**/) << 4000);
  dump(mn::int_style(10, 2, 4, true, true /* */) << 5000);
  dump(mn::int_style(10, 2, 4, true, true /**/) << -6000);
  dump(mn::int_style(10, 2, 4, true, false /**/) << 700);
  dump(mn::int_style(10, 2, 4, true, true /* */) << 800);
  dump(mn::int_style(10, 2, 4, true, true /**/) << -900);
  dump(mn::int_style(10, 2, 4, true, false /**/) << 10);
  dump(mn::int_style(10, 2, 4, true, true /* */) << 20);
  dump(mn::int_style(10, 2, 4, true, true /**/) << -30);
  dump(mn::int_style(10, 2, 4, true, false /**/) << 4);
  dump(mn::int_style(10, 2, 4, true, true /* */) << 5);
  dump(mn::int_style(10, 2, 4, true, true /**/) << -6);

  dump(mn::int_style(16, 1, 2, true, false) << 0x83);
  dump(mn::int_style(16, 1, 2, true, true) << 0x84);
  dump(mn::int_style(16, 1, 2, true, true) << -0x74);
  dump(mn::int_style(16, 1, 2, false, true) << -0x85);

  dump(mn::int_style(16, 2, 2, true, false) << 0x86);
  dump(mn::int_style(16, 2, 2, true, true) << 0x87);
  dump(mn::int_style(16, 2, 2, true, true) << -0x77);
  dump(mn::int_style(16, 2, 2, false, true) << 0x88);

  dump(mn::int_style(16, 3, 2, true, false) << 0x89);
  dump(mn::int_style(16, 3, 2, true, true) << 0x8a);
  dump(mn::int_style(16, 3, 2, true, true) << -0x7a);
  dump(mn::int_style(16, 3, 2, false, true) << 0x8b);

  dump(mn::int_style(16, 4, 2, true, false) << 0x8c);
  dump(mn::int_style(16, 4, 2, true, true) << 0x8d);
  dump(mn::int_style(16, 4, 2, true, true) << -0x7d);
  dump(mn::int_style(16, 4, 2, false, true) << 0x8e);

  dump(mn::int_style(16, 5, 2, true, false) << 0x8f);
  dump(mn::int_style(16, 5, 2, true, true) << 0x80);
  dump(mn::int_style(16, 5, 2, true, true) << -0x71);
  dump(mn::int_style(16, 5, 2, false, true) << 0x82);

  dump(mn::int_style(10, 5, 4, false, false /**/) << 0);
  dump(mn::int_style(10, 5, 4, true, false /**/) << 0);
  dump(mn::int_style(16, 5, 4, false, false /**/) << 0);
  dump(mn::int_style(16, 5, 4, true, false /**/) << 0);
  dump(mn::int_style(10, 5, 4, false, true /* */) << 0);
  dump(mn::int_style(10, 5, 4, true, true /* */) << 0);
  dump(mn::int_style(16, 5, 4, false, true /* */) << 0);
  dump(mn::int_style(16, 5, 4, true, true /* */) << 0);

  dump(mn::int_style(10, 5, 0, false, false /**/) << 0);
  dump(mn::int_style(10, 5, 0, true, false /**/) << 0);
  dump(mn::int_style(16, 5, 0, false, false /**/) << 0);
  dump(mn::int_style(16, 5, 0, true, false /**/) << 0);
  dump(mn::int_style(10, 5, 0, false, true /* */) << 0);
  dump(mn::int_style(10, 5, 0, true, true /* */) << 0);
  dump(mn::int_style(16, 5, 0, false, true /* */) << 0);
  dump(mn::int_style(16, 5, 0, true, true /* */) << 0);

  // non_copyable_and_non_movable_class
  CPP_DUMP_SET_OPTION(max_depth, 2);
  cpp_dump(
      mn::front(2) << mn::middle(1) << mn::back(2) << mn::both_ends(1)
                   << non_copyable_and_non_movable_class1
  );
  cpp_dump(non_copyable_and_non_movable_class_container1);
  cpp_dump(mn::front(1) << non_copyable_and_non_movable_class_container1);
  cpp_dump(container_of_non_copyable_non_const_iterator1);

  // unsupported_non_const_class
  cpp_dump(unsupported_container_of_supported_container_a1);
  cpp_dump(unsupported_container_of_supported_container_b1);
  cpp_dump(unsupported_non_const_class1);
}
