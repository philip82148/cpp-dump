// This is a test for competitive programming.
#include <bits/stdc++.h>
//
#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using namespace cpp_dump;

struct class_a {
  int int_a   = 314159265;
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

  non_copyable_and_non_movable_class(non_copyable_and_non_movable_class &&)           = delete;
  non_copyable_and_non_movable_class &operator=(non_copyable_and_non_movable_class &) = delete;

  non_copyable_and_non_movable_class() = delete;

  non_copyable_and_non_movable_class(const string &str_member) : str_member(str_member) {}
} non_copyable_and_non_movable_class1("This object has a pointer and reference_wrapper to itself.");

enum class enum_a { s, k, l };

CPP_DUMP_DEFINE_EXPORT_ENUM(enum_a, enum_a::s, enum_a::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(decltype(class_a1), int_a, long_b, a_str());
CPP_DUMP_DEFINE_EXPORT_OBJECT(non_copyable_and_non_movable_class, str_member, pointer, ref);

int main(int argc, char *argv[]) {
  if (argc != 4) return 1;
  int max_line_width = stoi(argv[1]);
  int max_depth      = stoi(argv[2]);
  int es_index       = stoi(argv[3]);

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
  int c_style_array[40]{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 3, 1, 4, 1, 5, 9, 2, 6, 5, 3,
                        3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
  CPP_DUMP(c_style_array);

  CPP_DUMP(vec);
  dump(vec);

  dump(c_style_array, c_style_array);

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
  multimap1.emplace('c', 31);
  multimap1.emplace('a', 12);
  multimap1.emplace('b', 23);
  multimap1.emplace('a', 44);
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
  CPP_DUMP(non_copyable_and_non_movable_class1);

  // enum
  enum_a enum_a_s = enum_a::s;
  enum_a enum_a_k = enum_a::k;
  enum_a enum_a_l = enum_a::l;
  CPP_DUMP(int_ptr, enum_a_s, enum_a_k, enum_a_l);

  // other
  bitset<10> bitset1(0xca);
  CPP_DUMP(bitset1);

  optional<int> optional1 = 1;
  CPP_DUMP(optional1);
  optional1 = nullopt;
  CPP_DUMP(optional1);
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

  // keep_*()
  array<int, 50> array50;
  map<int, int> map50;
  set<int> set50;
  rep(i, 50) {
    array50[i] = i;
    map50.emplace(i, i + 1);
    set50.emplace(i);
  }

  CPP_DUMP_SET_OPTION(max_iteration_count, 2);
  CPP_DUMP(array50, map50, set50);
  CPP_DUMP(keep_front() << array50, keep_front() << map50, keep_front() << set50);
  CPP_DUMP(keep_middle() << array50, keep_middle() << map50, keep_middle() << set50);
  CPP_DUMP(keep_back() << array50, keep_back() << map50, keep_back() << set50);
  CPP_DUMP(keep_both_ends() << array50, keep_both_ends() << map50, keep_both_ends() << set50);

  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  CPP_DUMP(keep_front(10) << array50, keep_front(10) << map50, keep_front(10) << set50);
  CPP_DUMP(keep_back(10) << array50, keep_back(10) << map50, keep_back(10) << set50);
  CPP_DUMP(keep_middle(10) << array50, keep_middle(10) << map50, keep_middle(10) << set50);
  CPP_DUMP(keep_both_ends(10) << array50, keep_both_ends(10) << map50, keep_both_ends(10) << set50);

  CPP_DUMP(keep_front(40) << array50, keep_front(40) << map50, keep_front(40) << set50);
  CPP_DUMP(keep_middle(40) << array50, keep_middle(40) << map50, keep_middle(40) << set50);
  CPP_DUMP(keep_back(40) << array50, keep_back(40) << map50, keep_back(40) << set50);
  CPP_DUMP(keep_both_ends(40) << array50, keep_both_ends(40) << map50, keep_both_ends(40) << set50);

  auto vec2 = vector<vector<vector<int>>>{
      {{1}},
      {{2, 6}},
      {
          {5, 6, 7},
          {3, 6, 8, 9},
          {5, 7, 8, 10, 11},
      }};

  CPP_DUMP(vec2, keep_back(2) << keep_both_ends(1) << keep_back(2) << vec2);
  CPP_DUMP(vec2, keep_both_ends(2) << keep_both_ends(2) << keep_both_ends(2) << vec2);

  CPP_DUMP(multimap1, keep_both_ends(2) << keep_both_ends(2) << multimap1);
  CPP_DUMP(multiset1, keep_middle(1) << keep_back(1) << multiset1);

  array<non_copyable_and_non_movable_class, 2> array_of_non_copyable_and_non_movable_class{
      {{string("value")}, {string("value")}}};

  CPP_DUMP(
      keep_front(2) << keep_middle(1) << keep_back(2) << keep_both_ends(1)
                    << non_copyable_and_non_movable_class1
  );
  CPP_DUMP(array_of_non_copyable_and_non_movable_class);
  CPP_DUMP(keep_front(1) << array_of_non_copyable_and_non_movable_class);
}
