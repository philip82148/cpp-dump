#include <bitset>
#include <complex>
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

int main() {
  int a          = 15;
  int *ptr       = &a;
  void *void_ptr = &a;
  std::vector<std::vector<int>> vector{{3, 5, 8, 9, 7}, {9, 3, 2, 3, 8}};
  std::set<char> set{'A', 'p', 'p', 'l', 'e'};
  std::map<int, int> map{{2, 6}, {4, 6}, {5, 3}};
  std::multiset<char> multiset{'A', 'p', 'p', 'l', 'e'};
  std::multimap<int, int> multimap{{2, 4}, {4, 6}, {5, 3}, {4, 7}};
  std::pair<int, char> pair{8, 'a'};
  std::tuple<int, double, std::string> tuple{7, 4.5, "This is a string."};
  std::queue<int> queue;
  std::priority_queue<int> pq;
  std::stack<int> stack;
  for (auto v : {1, 2, 3, 4, 5}) queue.push(v), pq.push(v), stack.push(v);
  std::bitset<8> bitset(0x3a);
  std::complex<double> complex{1.0, 1.0};
  std::optional<int> optional{15};
  std::variant<int, std::string> variant{"1"};

  std::clog << "\n// Basic Type" << std::endl;
  CPP_DUMP(false, 0, 0.0, '0'), CPP_DUMP(true, 3.14, a, 9265);
  CPP_DUMP("This is a string."), CPP_DUMP(ptr, void_ptr, nullptr);

  std::clog << "\n// Container" << std::endl;
  CPP_DUMP(vector);

  std::clog << "\n// Set/Map" << std::endl;
  CPP_DUMP(set), CPP_DUMP(map);

  std::clog << "\n// Multiset/Multimap" << std::endl;
  CPP_DUMP(multiset), CPP_DUMP(multimap);

  std::clog << "\n// Tuple" << std::endl;
  CPP_DUMP(tuple), CPP_DUMP(pair);

  std::clog << "\n// FIFO/LIFO" << std::endl;
  CPP_DUMP(queue), CPP_DUMP(pq), CPP_DUMP(stack);

  std::clog << "\n// Other" << std::endl;
  CPP_DUMP(bitset), CPP_DUMP(complex);
  CPP_DUMP(optional, std::nullopt), CPP_DUMP(variant);

  std::clog << std::endl;
}
