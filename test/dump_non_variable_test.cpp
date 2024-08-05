#include "../dump.hpp"
namespace cp = cpp_dump;

#ifdef USE_BITS_STDC
// This is a test for competitive programming.
#include <bits/stdc++.h>
#pragma message("<bits/stdc++.h> is used.")
#else
#include <array>
#include <bitset>
#include <climits>
#include <complex>
#include <iomanip>
#include <limits>
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
#include <type_traits>
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

  explicit non_copyable_and_non_movable_class(const string &s) : str_member(s) {}
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

struct tuple_of_non_copyable_and_non_movable_class {
  template <std::size_t i>
  auto &get() const {
    return non_copyable_and_non_movable_class1;
  }
} tuple_of_non_copyable_and_non_movable_class1;

namespace std {

template <>
struct tuple_size<tuple_of_non_copyable_and_non_movable_class> : integral_constant<std::size_t, 2> {
};

template <size_t i>
struct tuple_element<i, tuple_of_non_copyable_and_non_movable_class> {
  using type = non_copyable_and_non_movable_class;
};

}  // namespace std

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

struct non_copyable_non_movable_non_const_iterator {
  int index = 0;

  non_copyable_non_movable_non_const_iterator(non_copyable_non_movable_non_const_iterator &&) =
      delete;
  non_copyable_non_movable_non_const_iterator &
  operator=(non_copyable_non_movable_non_const_iterator &&) = delete;

  non_copyable_non_movable_non_const_iterator(const non_copyable_non_movable_non_const_iterator &) =
      delete;
  non_copyable_non_movable_non_const_iterator &
  operator=(const non_copyable_non_movable_non_const_iterator &) = delete;

  non_copyable_non_movable_non_const_iterator() = default;

  auto operator*() {
    return "This iterator is non-copyable and non-movable and doesn't support const.";
  }
  bool operator!=(const non_copyable_non_movable_non_const_iterator &) { return index < 2; }
  auto &operator++() {
    ++index;
    return *this;
  }
};

struct container_of_lvalue_iterator {
  mutable non_copyable_non_movable_non_const_iterator it;
  non_copyable_non_movable_non_const_iterator &begin() const {
    it.index = 0;
    return it;
  }
  non_copyable_non_movable_non_const_iterator &end() const { return it; }
} container_of_lvalue_iterator1;

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

CPP_DUMP_SET_OPTION_IN_GLOBAL(max_iteration_count, 100);

int main(int argc, char *argv[]) {
  if (argc != 2) return 1;
  auto es_style_ = (array{
      cp::es_style_t::no_es,
      cp::es_style_t::original,
      cp::es_style_t::by_syntax,
  }[stoi(argv[1])]);

  CPP_DUMP_SET_OPTION(es_style, es_style_);

  // Verify if CPP_DUMP_SET_OPTION_IN_GLOBAL is working correctly.
  cpp_dump(cpp_dump::max_iteration_count == 100);

  // basic
  cpp_dump(false, 0, 0.0, '0', (const char *)"0", string{"0"}, string_view{"0"});
  cpp_dump((signed char)0, (unsigned char)0, 0u, 0l, 0ul, 0ll, 0ull, 0.0f, 0.0l);
  cpp_dump(true, 3.14, 159265, "This is a test string");
  cpp_dump((signed char)3, (unsigned char)5, 8u, 9l, 7ul, 9ll, 3ull, 2.38f, 4.6l);
  cpp_dump("This contains newline\nhere.", R"(This contains ".)", R"(This contains `.)");
  cpp_dump("\n\n\n\n");

  // volatile
  volatile int volatile_int = 3589;
  volatile double volatile_double = 79.323;
  cpp_dump(volatile_int, volatile_double);

  // vector & auto indent
  cpp_dump((vector<int>{3, 1, 4}));
  cpp_dump((vector<vector<int>>{{3}, {1, 4}, {1, 5, 9}}));
  cpp_dump((vector<vector<vector<int>>>{{}, {{2, 6}}, {{5}, {3}, {5}}}));

  const vector<vector<int>> vec{{2, 4, 6, 7, 8, 9, 0, 1, 1, 1, 7, 8, 9, 0, 1, 1, 1, 1, 1, 1, 1}};

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
  cpp_dump((set<string>{"A", "p", "p", "l", "e", " ", "P", "e", "n"}));

  // multiset
  multiset<int> multiset1{3, 1, 4, 1};
  cpp_dump(multiset1);

  // FIFO/LIFO
  queue<int> queue1;
  for (auto v : {3, 2, 1}) {
    cpp_dump(queue1);
    queue1.push(v);
  }
  cpp_dump(queue1);

  priority_queue<char> priority_queue1;
  for (auto v : {'3', '2', '1'}) {
    cpp_dump(priority_queue1);
    priority_queue1.push(v);
  }
  cpp_dump(priority_queue1);

  stack<string> stack1;
  for (auto v : {"3", "2", "1"}) {
    cpp_dump(stack1);
    stack1.push(v);
  }
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

  complex<double> complex1{0.0, -1.0};
  cpp_dump(complex1);
  complex<double> complex2{10.0, 1.0};
  cpp_dump(complex2);

  logic_error logic_error1("This is a test error.");
  cpp_dump(logic_error1);

  // iterator
  cpp_dump(vec.begin());
  CPP_DUMP_SET_OPTION(enable_asterisk, true);
  cpp_dump(vec.begin());
  CPP_DUMP_SET_OPTION(enable_asterisk, false);

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
  cpp_dump(
      cp::es_style_t::no_es, cp::es_style_t::original, cp::es_style_t::by_syntax, cp::es_value
  );
  cpp_dump(
      cp::cont_indent_style_t::minimal,
      cp::cont_indent_style_t::when_nested,
      cp::cont_indent_style_t::when_non_tuples_nested,
      cp::cont_indent_style_t::always,
      cp::cont_indent_style
  );

  // index, front, back, middle, both_ends
  array<int, 50> array50;
  map<int, int> map50;
  set<int> set50;
  rep(i, 50) {
    array50[i] = i;
    map50[i] = i + 1;
    set50.insert(i);
  }

  CPP_DUMP_SET_OPTION(max_iteration_count, 20);
  cpp_dump(array50, map50, set50);

  cpp_dump(cp::front(10) << array50, cp::front(10) << map50, cp::front(10) << set50);
  cpp_dump(array50 | cp::front(10), map50 | cp::front(10), set50 | cp::front(10));

  cpp_dump(cp::back(10) << array50, cp::back(10) << map50, cp::back(10) << set50);
  cpp_dump(array50 | cp::back(10), map50 | cp::back(10), set50 | cp::back(10));

  cpp_dump(cp::middle(10) << array50, cp::middle(10) << map50, cp::middle(10) << set50);
  cpp_dump(array50 | cp::middle(10), map50 | cp::middle(10), set50 | cp::middle(10));

  cpp_dump(cp::both_ends(5) << array50, cp::both_ends(5) << map50, cp::both_ends(5) << set50);
  cpp_dump(array50 | cp::both_ends(5), map50 | cp::both_ends(5), set50 | cp::both_ends(5));

  vector<vector<vector<int>>> vec3d{
      {
          {1},
      },
      {
          {2, 6},
      },
      {
          {5, 6, 7},
          {3, 6, 8, 9},
          {5, 7, 8, 10, 11},
      }};

  cpp_dump(vec3d);

  cpp_dump(cp::back(2) << vec3d);
  cpp_dump(vec3d | cp::back(2));

  // cpp_dump(cp::back(2) << cp::both_ends(1) << vec3d);
  cpp_dump(vec3d | cp::back(2) | cp::both_ends(1));

  cpp_dump(cp::back(2) << cp::both_ends(1) << cp::middle(1) << vec3d);
  // cpp_dump(vec3d | cp::back(2) | cp::both_ends(1) | cp::middle(1));

  cpp_dump(cp::index() << cp::back(2) << cp::both_ends(1) << cp::middle(1) << vec3d);
  cpp_dump(vec3d | cp::back(2) | cp::both_ends(1) | cp::middle(1) | cp::index());

  cpp_dump(multimap1);
  cpp_dump(cp::both_ends(1) << cp::both_ends(1) << multimap1);
  cpp_dump(multimap1 | cp::both_ends(1) | cp::both_ends(1));

  cpp_dump(cp::both_ends(1) << cp::both_ends(1) << cp::index() << multimap1);
  cpp_dump(multimap1 | cp::both_ends(1) | cp::both_ends(1) | cp::index());

  cpp_dump(cp::middle(1) << cp::back(1) << multiset1);
  cpp_dump(multiset1 | cp::middle(1) | cp::back(1));

  multimap<vector<int>, vector<int>> multimap2{
      {{1, 2, 4, 2}, {11, 22, 11, 22}},
      {{1, 2, 4, 2}, {14, 21, 11, 22}},
      {{3, 4, 0}, {21, 5, 2, 3 - 8}},
      {{5, 6, 7, 8}, {14, -2}},
      {{1, 2, 4, 2}, {0, 5, 25}},
  };

  cpp_dump(cp::front(2) << cp::map_kv(cp::back(2), cp::front(2)) << multimap2);
  cpp_dump(multimap2 | cp::front(2) | cp::map_kv(cp::back(2), cp::front(2)));

  cpp_dump(
      cp::front(2) << cp::bin(0) << cp::map_kv(cp::hex(3, 3) << cp::back(2), cp::front(2))
                   << multimap2
  );
  cpp_dump(
      multimap2 | cp::front(2) | cp::bin(0) | cp::map_kv(cp::hex(3, 3) | cp::back(2), cp::front(2))
  );

  cpp_dump(cp::oct(3, 3) << cp::map_kv(cp::hex(3, 3) << cp::back(2), cp::front(2)) << multimap2);
  cpp_dump(multimap2 | cp::oct(3, 3) | cp::map_kv(cp::hex(3, 3) | cp::back(2), cp::front(2)));

  cpp_dump(
      cp::front(1) << cp::map_v(cp::oct(3, 3) << cp::middle(1) << cp::hex(3, 3) << cp::middle(2))
                   << multimap2
  );
  cpp_dump(
      multimap2 | cp::front(1)
      | cp::map_v(cp::oct(3, 3) | cp::middle(1) | cp::hex(3, 3) | cp::middle(2))
  );

  cpp_dump(
      multimap2 | cp::index() | cp::front(1)
      | cp::map_v(cp::oct(3, 3) | cp::middle(1) | cp::hex(3, 3) | cp::middle(2))
  );

  cpp_dump(
      multimap2 | cp::front(1) | cp::index()
      | cp::map_v(cp::oct(3, 3) | cp::middle(1) | cp::hex(3, 3) | cp::middle(2))
  );

  cpp_dump(
      multimap2 | cp::front(1)
      | cp::map_v(cp::oct(3, 3) | cp::middle(1) | cp::hex(3, 3) | cp::middle(2) | cp::index())
  );

  // cp::int_style()
  for (auto base : {0, 1, 2, 8, 10, 16, 17}) cpp_dump(base, cp::int_style(base, -1, 2) << 0x10000);

  vector<vector<int>> vec2d = {
      {-1, 2, 3, -4, 5, 6, 7, -8},
      {9, -10, 11, -12, 13},
      {14, 15},
      {15, -16},
      {-17, 18},
      {19, 20},
  };

  cpp_dump(vec2d | cp::index());
  cpp_dump(vec2d | cp::front(3) | cp::index());
  cpp_dump(vec2d | cp::back(3) | cp::index());
  cpp_dump(vec2d | cp::middle(3) | cp::index());
  cpp_dump(vec2d | cp::both_ends(1) | cp::index());
  cpp_dump(vec2d | cp::front() | cp::front(3) | cp::index());
  cpp_dump(vec2d | cp::front() | cp::back(3) | cp::index());
  cpp_dump(vec2d | cp::front() | cp::middle(3) | cp::index());
  cpp_dump(vec2d | cp::front() | cp::both_ends(1) | cp::index());

  cpp_dump(cp::dec(2) << vec2d);
  cpp_dump(vec2d | cp::dec(2, 0, false));

  auto true_ = true;
  auto true_a = true;
  auto true_aa = true;
  auto false_ = false;
  cpp_dump(cp::dec(8, 4, false) << 1u);
  cpp_dump(cp::dec(8, 4, true_) << 1u);
  cpp_dump(cp::dec(8, 4, false_) << 2);
  cpp_dump(cp::dec(8, 4, true_a) << 2);
  cpp_dump(cp::dec(8, 4, false) << -3);
  cpp_dump(cp::dec(8, 4, true_) << -3);

  cpp_dump(cp::dec(9, 4, false) << 4u);
  cpp_dump(cp::dec(9, 4, true_) << 4u);
  cpp_dump(cp::dec(9, 4, false_) << 5);
  cpp_dump(cp::dec(9, 4, true_a) << 5);
  cpp_dump(cp::dec(9, 4, false) << -6);
  cpp_dump(cp::dec(9, 4, true_) << -6);

  cpp_dump(cp::dec(7, 4, false) << 7u);
  cpp_dump(cp::dec(7, 4, true_) << 7u);
  cpp_dump(cp::dec(7, 4, false_) << 8);
  cpp_dump(cp::dec(7, 4, true_a) << 8);
  cpp_dump(cp::dec(7, 4, false) << -9);
  cpp_dump(cp::dec(7, 4, true_) << -9);

  cpp_dump(cp::dec(4, 4, false) << 7u);
  cpp_dump(cp::dec(4, 4, true_) << 7u);
  cpp_dump(cp::dec(4, 4, false_) << 8);
  cpp_dump(cp::dec(4, 4, true_a) << 8);
  cpp_dump(cp::dec(4, 4, false) << -9);
  cpp_dump(cp::dec(4, 4, true_) << -9);

  cpp_dump(cp::dec(5, 4, false) << 0u);
  cpp_dump(cp::dec(5, 4, true_) << 0u);
  cpp_dump(cp::dec(5, 4, false_) << 1);
  cpp_dump(cp::dec(5, 4, true_a) << 1);
  cpp_dump(cp::dec(5, 4, false) << -2);
  cpp_dump(cp::dec(5, 4, true_) << -2);

  cpp_dump(cp::dec(5, 4, true) << 10000u);
  cpp_dump(cp::dec(5, 4, true_) << 4000u);
  cpp_dump(cp::dec(5, 4, true_a) << 700u);
  cpp_dump(cp::dec(5, 4, true_) << 20000);
  cpp_dump(cp::dec(5, 4, true) << -30000);
  cpp_dump(cp::dec(5, 4, true_a) << 5000);
  cpp_dump(cp::dec(5, 4, true_) << -6000);
  cpp_dump(cp::dec(5, 4, true_aa) << 800);
  cpp_dump(cp::dec(5, 4, true_a) << -900);

  cpp_dump(cp::dec(0, 4, true) << 10000u);
  cpp_dump(cp::dec(0, 4, true_) << 4000u);
  cpp_dump(cp::dec(0, 4, true_a) << 700u);
  cpp_dump(cp::dec(0, 4, true_) << 20000);
  cpp_dump(cp::dec(0, 4, true_a) << 5000);
  cpp_dump(cp::dec(0, 4, true_) << -6000);
  cpp_dump(cp::dec(0, 4, true_aa) << 800);
  cpp_dump(cp::dec(0, 4, true_a) << -900);

  cpp_dump(cp::hex(0, 2, true_) << 0x83u);
  cpp_dump(cp::hex(0, 2, false) << 0x85u);
  cpp_dump(cp::hex(0, 2, true_a) << 0x84);
  cpp_dump(cp::hex(0, 2, false_) << 0x85);
  cpp_dump(cp::hex(0, 2, true_) << -0x74);
  cpp_dump(cp::hex(0, 2, false) << -0x85);

  cpp_dump(cp::hex(2, 2, true_) << 0x86u);
  cpp_dump(cp::hex(2, 2, false) << 0x89u);
  cpp_dump(cp::hex(2, 2, true_a) << 0x87);
  cpp_dump(cp::hex(2, 2, false_) << 0x88);
  cpp_dump(cp::hex(2, 2, true_) << -0x77);
  cpp_dump(cp::hex(2, 2, false) << -0x89);

  cpp_dump(cp::hex(3, 2, true_) << 0x89u);
  cpp_dump(cp::hex(3, 2, false) << 0x8bu);
  cpp_dump(cp::hex(3, 2, true_a) << 0x8a);
  cpp_dump(cp::hex(3, 2, false_) << 0x8b);
  cpp_dump(cp::hex(3, 2, true_) << -0x7a);
  cpp_dump(cp::hex(3, 2, false) << -0x8b);

  cpp_dump(cp::hex(4, 2, true_) << 0x8cu);
  cpp_dump(cp::hex(4, 2, false) << 0x8eu);
  cpp_dump(cp::hex(4, 2, true_a) << 0x8d);
  cpp_dump(cp::hex(4, 2, false_) << 0x8e);
  cpp_dump(cp::hex(4, 2, true_) << -0x7d);
  cpp_dump(cp::hex(4, 2, false) << -0x8e);

  cpp_dump(cp::hex(5, 2, true_) << 0x8fu);
  cpp_dump(cp::hex(5, 2, false) << 0x82u);
  cpp_dump(cp::hex(5, 2, true_a) << 0x80);
  cpp_dump(cp::hex(5, 2, false_) << 0x82);
  cpp_dump(cp::hex(5, 2, true_) << -0x71);
  cpp_dump(cp::hex(5, 2, false) << -0x82);

  cpp_dump(cp::hex(5, 4, false) << 0x389u);
  cpp_dump(cp::hex(5, 4, true_) << 0x389u);
  cpp_dump(cp::hex(5, 4, false) << 0x289u);
  cpp_dump(cp::hex(5, 4, true_) << 0x982u);
  cpp_dump(cp::hex(5, 4, false_) << 0x902);
  cpp_dump(cp::hex(5, 4, true_a) << 0x382);
  cpp_dump(cp::hex(5, 4, false_) << 0x291);
  cpp_dump(cp::hex(5, 4, true_a) << 0x027);

  cpp_dump(cp::hex(5, 0, false) << 0x9283u);
  cpp_dump(cp::hex(5, 0, true_) << 0x8282u);
  cpp_dump(cp::hex(5, 0, false) << 0x8282u);
  cpp_dump(cp::hex(5, 0, true_) << 0x9192u);
  cpp_dump(cp::hex(5, 0, false_) << 0x0920);
  cpp_dump(cp::hex(5, 0, true_a) << 0x0923);
  cpp_dump(cp::hex(5, 0, false_) << 0x0291);
  cpp_dump(cp::hex(5, 0, true_a) << 0x0939);

  CPP_DUMP_SET_OPTION(print_expr, false);
  rep(i, 64) {
    cpp_dump(((1ULL << i) - 1) | cp::bin(64, 0));
    cpp_dump((1ULL << i) | cp::bin(64, 0));
  }
  CPP_DUMP_SET_OPTION(print_expr, true);

  auto dump_limits = [](auto t, std::string_view type_name) {
    using T = decltype(t);
    using UnsignedT = make_unsigned_t<T>;
    T m_1_a = -1;
    T m_1_ = -1;
    T max_a = std::numeric_limits<T>::max();
    T max_ = std::numeric_limits<T>::max();
    T min_a = std::numeric_limits<T>::min();
    T min_ = std::numeric_limits<T>::min();
    UnsignedT umax_ = std::numeric_limits<UnsignedT>::max();
    UnsignedT umax = std::numeric_limits<UnsignedT>::max();

    clog << type_name << " cp::bin(), cp::oct()" << endl;

    cpp_dump(m_1_a | cp::bin(), m_1_a | cp::oct());
    cpp_dump(max_a | cp::bin(), max_a | cp::oct());
    cpp_dump(min_a | cp::bin(), min_a | cp::oct());
    cpp_dump(umax_ | cp::bin(), umax_ | cp::oct());

    cpp_dump(m_1_ | cp::ubin(), m_1_ | cp::uoct());
    cpp_dump(max_ | cp::ubin(), max_ | cp::uoct());
    cpp_dump(min_ | cp::ubin(), min_ | cp::uoct());
    cpp_dump(umax | cp::ubin(), umax | cp::uoct());

    clog << type_name << " cp::hex(), cp::dec()" << endl;

    cpp_dump(m_1_a | cp::hex(), m_1_a | cp::dec());
    cpp_dump(max_a | cp::hex(), max_a | cp::dec());
    cpp_dump(min_a | cp::hex(), min_a | cp::dec());
    cpp_dump(umax_ | cp::hex(), umax_ | cp::dec());

    cpp_dump(m_1_ | cp::uhex(), m_1_ | cp::udec());
    cpp_dump(max_ | cp::uhex(), max_ | cp::udec());
    cpp_dump(min_ | cp::uhex(), min_ | cp::udec());
    cpp_dump(umax | cp::uhex(), umax | cp::udec());
  };

  dump_limits(int8_t{}, "int8_t");
  dump_limits(int16_t{}, "int16_t");
  dump_limits(int32_t{}, "int32_t");
  dump_limits(int64_t{}, "int64_t");

  // format
  cpp_dump(3.14159265f | cp::format("%.3f"));
  cpp_dump(3.141592653589793L | cp::format("%.15Lf"));
  cpp_dump(314159265 | cp::format("%020d"));
  cpp_dump(31415926535ll | cp::format("%020lld"));
  cpp_dump(complex1 | cp::format("%.10f"));

  // bw, boolnum
  vector<vector<bool>> vec5 = {
      {true, false, true, false},
      {false, false, false, false},
      {true, true, true, true},
  };

  cpp_dump(vec5);
  cpp_dump(vec5 | cp::bw());
  cpp_dump(vec5 | cp::bw(true));
  cpp_dump(vec5 | cp::boolnum());
  cpp_dump(vec5 | cp::hex() | cp::bw());
  cpp_dump(vec5 | cp::hex() | cp::bw(true));
  cpp_dump(vec5 | cp::hex() | cp::boolnum());

  // charhex
  std::string_view sv1 = "Pi=3.14...";
  vector<char> vec6(sv1.begin(), sv1.end());
  cpp_dump(vec6);
  cpp_dump(vec6 | cp::charhex());
  cpp_dump(vec6 | cp::bin() | cp::charhex());

  // lvalue export_command
  auto index = cp::index();
  auto back = cp::back(3);
  auto vec4back = vec2d | cp::back(3);
  cpp_dump(vec2d | (back | cp::index()));
  cpp_dump(vec2d | (cp::index() | back));
  cpp_dump(vec2d | (back | index));
  cpp_dump(vec2d | back | index);
  cpp_dump(vec4back | index);

  // non_copyable_and_non_movable_class
  CPP_DUMP_SET_OPTION(max_depth, 2);
  cpp_dump(
      cp::front(2) << cp::middle(1) << cp::back(2) << cp::both_ends(1)
                   << non_copyable_and_non_movable_class1
  );
  cpp_dump(non_copyable_and_non_movable_class_container1);
  cpp_dump(cp::front(1) << non_copyable_and_non_movable_class_container1);
  cpp_dump(tuple_of_non_copyable_and_non_movable_class1);
  cpp_dump(container_of_non_copyable_non_const_iterator1);
  cpp_dump(container_of_lvalue_iterator1);

  // unsupported_non_const_class
  cpp_dump(unsupported_container_of_supported_container_a1);
  cpp_dump(unsupported_container_of_supported_container_b1);
  cpp_dump(unsupported_non_const_class1);
}
