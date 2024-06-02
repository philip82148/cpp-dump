#include "../dump.hpp"
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
CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(int_b, str);
CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(str_member, pointer, ref);

int main(int argc, char *argv[]) {
  if (argc != 4) return 1;
  unsigned int max_line_width_ = stoi(argv[1]);
  unsigned int max_depth_ = stoi(argv[2]);
  unsigned int es_index = stoi(argv[3]);

  CPP_DUMP_SET_OPTION(max_line_width, max_line_width_);
  CPP_DUMP_SET_OPTION(max_depth, max_depth_);
  CPP_DUMP_SET_OPTION(max_iteration_count, 100);
  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
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

  vector<string> vector_of_string{this_string_contains_new_line_here};
  cpp_dump(vector_of_string);

  draw_line("test of 2d vector");
  vector<long long> vector2d1d1 = vec1d1;
  vector2d1d1.push_back(900000);

  vector<vector<long long>> vector2d1{vector2d1d1};
  cpp_dump(vector2d1);

  vector<long long> vector2d1d2 = vec1d1;
  vector2d1d2.push_back(9000000);

  vector<vector<long long>> vector2d2{vector2d1d2};
  cpp_dump(vector2d2);

  draw_line("test of set");
  set<long long> set1d2(vec1d2.begin(), vec1d2.end());
  cpp_dump(set1d2);
  set<long long> set1d3{vec1d3.begin(), vec1d3.end()};
  cpp_dump(set1d3);

  set<string> set_of_string{this_string_contains_new_line_here};
  cpp_dump(set_of_string);

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
  self_reference_class self_reference_class1{"This has a self-reference."};
  cpp_dump(self_reference_class1);
  self_reference_class self_reference_class2{"This has a self-reference.."};
  cpp_dump(self_reference_class2);
}
