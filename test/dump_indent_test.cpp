#include "../cpp_dump.hpp"
namespace cp = cpp_dump;

#ifdef USE_BITS_STDC
// This is a test for competitive programming.
#include <bits/stdc++.h>
#pragma message("<bits/stdc++.h> is used.")
#else
#include <array>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#endif

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;

struct class_a {
  long long int_a;
  string str;
};

struct class_b {
  long long int_b;
  string str;
};

struct self_reference_class {
  string str_member;
  self_reference_class *pointer{this};
  reference_wrapper<self_reference_class> ref{*this};
};

CPP_DUMP_DEFINE_EXPORT_OBJECT(class_a, int_a, str);
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(int_b, str);
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(str_member, pointer, ref);

#define PRINT(x) (x), (clog << #x ";" << endl)

int main(int argc, char *argv[]) {
  if (argc != 5) return 1;
  unsigned int max_line_width_ = stoi(argv[1]);
  unsigned int max_depth_ = stoi(argv[2]);
  auto es_style_ = (array{
      cp::types::es_style_t::no_es,
      cp::types::es_style_t::original,
      cp::types::es_style_t::by_syntax,
  }[stoi(argv[3])]);
  bool detailed_es = es_style_ == cp::types::es_style_t::by_syntax;
  bool color_test = stoi(argv[4]);

  CPP_DUMP_SET_OPTION(max_line_width, max_line_width_);
  CPP_DUMP_SET_OPTION(max_depth, max_depth_);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
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

  auto draw_line = [=](std::string comment = "") {
    string line_with_comment(max_line_width_, '=');

    if (!comment.empty()) {
      if (max_line_width_ >= comment.size() + 10) {
        comment = "  " + comment + "  ";
        line_with_comment.replace((max_line_width_ - comment.size()) / 2, comment.size(), comment);
      } else {
        line_with_comment += "  --  " + comment;
      }
    }

    clog << line_with_comment << endl;
  };

  constexpr auto this_string_contains_new_line_here = "this string contains a newline\nhere";

  draw_line("test of cpp_dump_macro");
  cpp_dump(10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 13, 14, 00);
  cpp_dump(10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 13, 14, 000);
  cpp_dump(10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 13, 14, 00, 15);
  cpp_dump(
      10000000000,
      20000000000,
      30000000000,
      40000000000,
      50000000000,
      00000000,
      70000000000,
      80000000000,
      90000000000,
      100000000000,
      110000000000,
      0000,
      130000000000,
      140000000000,
      150000000000,
      160000000000
  );
  cpp_dump(string(132, '-'));
  cpp_dump(string(131, '-'));
  cpp_dump(string(131, '-'), string(131, '-'));

  draw_line("test of value contains newline(s)");
  cpp_dump("\n", 0, 1, 2);
  cpp_dump(0, 1, 2, "\n");
  cpp_dump(
      string(131, '-'),
      R"(this string contains a newline
here.)"[0],
      2
  );
  cpp_dump(R"(this string contains a newline
here.)");

  draw_line("test of 1d vector");
  vector<long long> vec1d1{
      10000000000000000,
      20000000000000000,
      30000000000000000,
      40000000000000000,
      50000000000000000,
      60000000000000000,
      70000000000000000,
      800000,
  };
  vector<long long> vec1d2{
      10000000000000000,
      20000000000000000,
      30000000000000000,
      40000000000000000,
      50000000000000000,
      60000000000000000,
      70000000000000000,
      80000000000,
  };
  cpp_dump(vec1d1, vec1d2);
  vector<long long> vec1d3{
      10000000000000000,
      20000000000000000,
      30000000000000000,
      40000000000000000,
      50000000000000000,
      60000000000000000,
      70000000000000000,
      800000000000,
  };
  cpp_dump(vec1d3);

  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::always));
  cpp_dump(vec1d1);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_nested));

  vector<string> vector_of_string{this_string_contains_new_line_here};
  cpp_dump(vector_of_string);

  draw_line("test of 2d vector");
  vector<long long> vec1d_for_2d1 = vec1d1;
  vec1d_for_2d1.push_back(900000);

  vector<vector<long long>> vector2d1{vec1d_for_2d1};
  cpp_dump(vector2d1);

  vector<long long> vec1d_for_2d2 = vec1d1;
  vec1d_for_2d2.push_back(9000000);

  vector<vector<long long>> vector2d2{vec1d_for_2d2};
  cpp_dump(vector2d2);

  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  vector<long long> vec1d_for_2d3 = {
      10000000000000000,
      20000000000000000,
      30000000000000000,
      40000000000000000,
      50000000000000000,
      60000000000000000,
      70000000000000000,
      8000000,
  };
  vector<vector<long long>> vector2d3{vec1d_for_2d3};
  cpp_dump(vector2d3);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::minimal));
  cpp_dump(vector2d3);

  vector<long long> vec1d_for_2d4 = {
      10000000000000000,
      20000000000000000,
      30000000000000000,
      40000000000000000,
      50000000000000000,
      60000000000000000,
      70000000000000000,
      80000000,
  };
  vector<vector<long long>> vector2d4{vec1d_for_2d4};
  cpp_dump(vector2d4);
  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  vector<tuple<long long, long long>> vec_of_tuple{{1, 2}};
  cpp_dump(vec_of_tuple);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_nested));
  cpp_dump(vec_of_tuple);

  draw_line("test of set");
  set<long long> set1d2(vec1d2.begin(), vec1d2.end());
  cpp_dump(set1d2);
  set<long long> set1d3{vec1d3.begin(), vec1d3.end()};
  cpp_dump(set1d3);

  set<string> set_of_string{this_string_contains_new_line_here};
  cpp_dump(set_of_string);

  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  set<vector<long long>> set_of_vec1{vec1d_for_2d3};
  cpp_dump(set_of_vec1);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::minimal));
  cpp_dump(set_of_vec1);

  set<vector<long long>> set_of_vec2{vec1d_for_2d4};
  cpp_dump(set_of_vec2);
  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  set<tuple<long long, long long>> set_of_tuple{{1, 2}};
  cpp_dump(set_of_tuple);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_nested));
  cpp_dump(set_of_tuple);

  draw_line("test of multiset");
  multiset<long long> mset2({
      1000000000000,
      2000000000000,
      3000000000000,
      4000000000000,
      5000000000000,
      6000000000000,
      7000000000000,
      8000000,
  });
  cpp_dump(mset2);
  multiset<long long> mset3{{
      1000000000000,
      2000000000000,
      3000000000000,
      4000000000000,
      5000000000000,
      6000000000000,
      7000000000000,
      80000000,
  }};
  cpp_dump(mset3);

  multiset<string> mset_of_string{this_string_contains_new_line_here};
  cpp_dump(mset_of_string);

  draw_line("test of map");
  map<long long, long long> map1{
      {0, 800000000000000},
      {1000000000000000000, 2000000000000000000},
      {3000000000000000000, 4000000000000000000},
      {5000000000000000000, 6000000000000000000},
  };
  cpp_dump(map1);
  map<long long, long long> map2{
      {0, 8000000000000000},
      {1000000000000000000, 2000000000000000000},
      {3000000000000000000, 4000000000000000000},
      {5000000000000000000, 6000000000000000000},
  };
  cpp_dump(map2);
  map<string, long long> map_of_string_key{
      {this_string_contains_new_line_here, 800000000000000},
  };
  cpp_dump(map_of_string_key);
  map<long long, string> map_of_string_value{
      {800000000000000, this_string_contains_new_line_here},
  };
  cpp_dump(map_of_string_value);

  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  vector<long long> vec1d_for_map_of_vec1 = {
      1000000000000000,
      2000000000000000,
      3000000000000000,
      400000000000000,
      500000000000000,
      600000000000000,
      700000000000000,
      800000000000000,
  };
  map<vector<long long>, long long> map_of_vec_key1{
      {vec1d_for_map_of_vec1, 1},
  };
  cpp_dump(map_of_vec_key1);
  map<long long, vector<long long>> map_of_vec_value1{
      {1, vec1d_for_map_of_vec1},
  };
  cpp_dump(map_of_vec_value1);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::minimal));
  cpp_dump(map_of_vec_key1);
  cpp_dump(map_of_vec_value1);

  vector<long long> vec1d_for_map_of_vec2 = {
      1000000000000000,
      2000000000000000,
      3000000000000000,
      4000000000000000,
      500000000000000,
      600000000000000,
      700000000000000,
      800000000000000,
  };
  map<vector<long long>, long long> map_of_vec_key2{
      {vec1d_for_map_of_vec2, 1},
  };
  cpp_dump(map_of_vec_key2);
  map<long long, vector<long long>> map_of_vec_value2{
      {1, vec1d_for_map_of_vec2},
  };
  cpp_dump(map_of_vec_value2);
  PRINT(
      CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_non_tuples_nested)
  );
  map<long long, tuple<long long, long long>> map_of_tuple_key{{1, {2, 3}}};
  cpp_dump(map_of_tuple_key);
  map<tuple<long long, long long>, long long> map_of_tuple_value{{{1, 2}, 3}};
  cpp_dump(map_of_tuple_value);
  PRINT(CPP_DUMP_SET_OPTION(cont_indent_style, cp::types::cont_indent_style_t::when_nested));
  cpp_dump(map_of_tuple_key);
  cpp_dump(map_of_tuple_value);

  draw_line("test of multimap");
  multimap<long long, long long> mmap1{
      {0, 10000000000000000},
      {0, 20000000000000000},
      {0, 30000000000000000},
      {0, 40000000000000000},
      {0, 50000000000000000},
      {0, 60000000000000000},
      {0, 70000000000000000},
      {0, 8000000},
  };
  cpp_dump(mmap1);
  multimap<long long, long long> mmap2{
      {0, 10000000000000000},
      {0, 20000000000000000},
      {0, 30000000000000000},
      {0, 40000000000000000},
      {0, 50000000000000000},
      {0, 60000000000000000},
      {0, 70000000000000000},
      {0, 80000000},
  };
  cpp_dump(mmap2);
  multimap<string, long long> mmap_of_string_key{
      {this_string_contains_new_line_here, 800000000000000},
  };
  cpp_dump(mmap_of_string_key);
  multimap<long long, string> mmap_of_string_value{
      {800000000000000, this_string_contains_new_line_here},
  };
  cpp_dump(mmap_of_string_value);

  draw_line("test of tuple");
  tuple<long long, long long, long long, long long, long long, long long, long long, long long>
      tuple1d2{
          10000000000000000,
          20000000000000000,
          30000000000000000,
          40000000000000000,
          50000000000000000,
          60000000000000000,
          70000000000000000,
          80000000000,
      };
  cpp_dump(tuple1d2);
  tuple<long long, long long, long long, long long, long long, long long, long long, long long>
      tuple1d3{
          10000000000000000,
          20000000000000000,
          30000000000000000,
          40000000000000000,
          50000000000000000,
          60000000000000000,
          70000000000000000,
          800000000000,
      };
  cpp_dump(tuple1d3);
  draw_line();
  pair<string, string> pair_of_string1{string(68, '-'), string(70, '-')};
  cpp_dump(pair_of_string1);
  pair<string, string> pair_of_string2{string(68, '-'), string(71, '-')};
  cpp_dump(pair_of_string2);

  pair<string, string> pair_of_string3{this_string_contains_new_line_here, ""};
  cpp_dump(pair_of_string3);

  draw_line("test of FIFO/LIFO");
  queue<string> queue1;
  queue1.push(string(112, '-'));
  cpp_dump(queue1);
  queue<string> queue2;
  queue2.push(string(113, '-'));
  cpp_dump(queue2);
  queue<string> queue3;
  queue3.push(this_string_contains_new_line_here);
  cpp_dump(queue3);
  queue<string> queue4;
  queue4.push(string(50, '-'));
  queue4.push(string(50, '-'));
  cpp_dump(queue4);
  queue<string> queue5;
  queue5.push(string(50, '-'));
  queue5.push(string(51, '-'));
  cpp_dump(queue5);

  draw_line();
  stack<string> stack1;
  stack1.push(string(114, '-'));
  cpp_dump(stack1);
  stack<string> stack2;
  stack2.push(string(115, '-'));
  cpp_dump(stack2);
  stack<string> stack3;
  stack3.push(this_string_contains_new_line_here);
  cpp_dump(stack3);

  draw_line();
  priority_queue<string> pq1;
  pq1.push(string(105, '-'));
  cpp_dump(pq1);
  priority_queue<string> pq2;
  pq2.push(string(106, '-'));
  cpp_dump(pq2);
  priority_queue<string> pq3;
  pq3.push(this_string_contains_new_line_here);
  cpp_dump(pq3);

  draw_line("test of CPP_DUMP_DEFINE_EXPORT_OBJECT()");
  class_a class_a1{1000000000000000000, string(102, '-')};
  cpp_dump(class_a1);
  class_a class_a2{1000000000000000000, string(103, '-')};
  cpp_dump(class_a2);
  class_a class_a3{0, this_string_contains_new_line_here};
  cpp_dump(class_a3);

  class_b class_b1{1000000000000000000, string(102, '-')};
  cpp_dump(class_b1);
  class_b class_b2{1000000000000000000, string(103, '-')};
  cpp_dump(class_b2);
  class_b class_b3{0, this_string_contains_new_line_here};
  cpp_dump(class_b3);

  draw_line("test of self-reference-class");
  self_reference_class self_reference_class1{
      "This is self_reference_class, which has a self-reference."};
  cpp_dump(self_reference_class1);
  self_reference_class self_reference_class2{
      "This is self_reference_class, which has a self-reference.."};
  cpp_dump(self_reference_class2);
}
