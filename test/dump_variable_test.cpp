#include <array>
#include <unordered_map>
#include <unordered_set>
//
#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using namespace cpp_dump;

int main(int argc, char *argv[]) {
  if (argc != 3) return 1;
  bool compiler_dependent = static_cast<bool>(stoi(argv[1]));
  bool es_index = static_cast<bool>(stoi(argv[2]));

  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
  );

  if (compiler_dependent) {
    class original_error : public logic_error {
      using logic_error::logic_error;
    } original_error1("This is an original error.");
    cpp_dump(original_error1);
  } else {
    // pointer
    const void *void_ptr = (void *)0x7ffd06586204;
    cpp_dump(void_ptr);

    // unordered
    cpp_dump((unordered_map<int, int>{{4, 6}, {2, 6}, {4, 3}}));

    unordered_multimap<char, int> unordered_multimap1;
    unordered_multimap1.emplace('c', 30);
    unordered_multimap1.emplace('a', 10);
    unordered_multimap1.emplace('b', 20);
    unordered_multimap1.emplace('a', 40);
    cpp_dump(unordered_multimap1);

    cpp_dump((unordered_set{3, 1, 4, 1, 5}));

    unordered_multiset<int> unordered_multiset1;
    unordered_multiset1.insert(3);
    unordered_multiset1.insert(1);
    unordered_multiset1.insert(4);
    unordered_multiset1.insert(1);
    cpp_dump(unordered_multiset1);
  }
}
