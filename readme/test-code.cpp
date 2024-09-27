#include <bitset>
#include <complex>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "../cpp-dump.hpp"
namespace cp = cpp_dump;

int main() {
  bool my_bool = true;
  double my_double = 3.141592;
  int my_int = 65;
  char my_char = 'a', LF_char = '\n';
  std::string my_string = "This is a string.";
  int *int_ptr = &my_int;
  void *void_ptr = &my_int;
  std::vector<std::vector<int>> my_vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};
  std::set<char> my_set{'A', 'p', 'p', 'l', 'e'};
  std::map<int, int> my_map{{2, 6}, {4, 6}, {5, 3}};
  std::multiset<char> my_multiset{'A', 'p', 'p', 'l', 'e'};
  std::multimap<int, int> my_multimap{{2, 4}, {4, 6}, {5, 3}, {4, 7}};
  std::pair<int, char> my_pair{8, 'a'};
  std::tuple<int, double, std::string> my_tuple{7, 4.5, "This is a string."};
  std::queue<int> my_queue;
  std::priority_queue<int> my_priority_queue;
  std::stack<int> my_stack;
  for (auto v : {1, 2, 3, 4, 5}) my_queue.push(v), my_priority_queue.push(v), my_stack.push(v);
  std::bitset<8> my_bitset(0x3a);
  std::complex<double> my_complex{1.0, -1.0};
  std::optional<int> my_optional{15};
  std::variant<int, std::string> my_variant{"This is a string."};
  std::vector<std::pair<int, std::string>> vector_of_pairs{{1, "apple"}, {3, "banana"}};

  CPP_DUMP_SET_OPTION(max_line_width, 100);
  CPP_DUMP_SET_OPTION(max_iteration_count, 10);

  std::clog << "\n// Basic Type" << std::endl;
  cpp_dump(my_bool, my_double, my_int), cpp_dump(my_string, my_char, LF_char);
  cpp_dump(int_ptr, void_ptr, nullptr);

  std::clog << "\n// Container" << std::endl;
  cpp_dump(my_vector);

  std::clog << "\n// Set/Map" << std::endl;
  cpp_dump(my_set), cpp_dump(my_map);

  std::clog << "\n// Multiset/Multimap" << std::endl;
  cpp_dump(my_multiset), cpp_dump(my_multimap);

  std::clog << "\n// Tuple" << std::endl;
  cpp_dump(my_tuple), cpp_dump(my_pair);

  std::clog << "\n// FIFO/LIFO" << std::endl;
  cpp_dump(my_queue), cpp_dump(my_priority_queue), cpp_dump(my_stack);

  std::clog << "\n// Other" << std::endl;
  cpp_dump(my_bitset), cpp_dump(my_complex);
  cpp_dump(my_optional, std::nullopt), cpp_dump(my_variant);

  std::clog << "\n// Combination" << std::endl;
  cpp_dump(vector_of_pairs);

  std::vector<std::vector<int>> some_huge_vector(100, std::vector<int>(100));
  std::vector<std::vector<unsigned int>> unsigned_int_vector(100, std::vector<unsigned int>(100));

  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      some_huge_vector[i][j] = ((i + 1) * 7 % 19 - 9) * ((j + 1) * 3 % 19 - 9);
      unsigned_int_vector[i][j] = std::abs(some_huge_vector[i][j]);
    }
  }

  std::clog << "\n// Manipulator Test" << std::endl;
  cpp_dump(some_huge_vector | cp::back() | cp::both_ends() | cp::dec(2));
  cpp_dump(some_huge_vector | cp::dec(2) | cp::index());
}
