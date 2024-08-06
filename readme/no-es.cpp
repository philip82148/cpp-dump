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

#include "../dump.hpp"
namespace cp = cpp_dump;

int main() {
  CPP_DUMP_SET_OPTION(es_style, cp::es_style_t::no_es);

  int my_int = 15;
  int *ptr = &my_int;
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
  std::complex<double> my_complex{1.0, 1.0};
  std::optional<int> my_optional{15};
  std::variant<int, std::string> my_variant{"1"};
  std::vector<std::pair<int, std::string>> vector_of_pairs{{1, "apple"}, {3, "banana"}};

  std::clog << "\n// Basic Type" << std::endl;
  cpp_dump(false, 0, 0.0), cpp_dump(true, 3.14, my_int, -9265);
  cpp_dump("This is a string.", 'a', '\n'), cpp_dump(ptr, void_ptr, nullptr);

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

  std::clog << std::endl;
}
