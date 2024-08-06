#include <cmath>
#include <iostream>
#include <vector>

#include "../dump.hpp"
namespace cp = cpp_dump;

int main() {
  std::clog << std::endl;

  std::vector<std::vector<int>> some_huge_vector(100, std::vector<int>(100));
  std::vector<std::vector<unsigned int>> unsigned_int_vector(100, std::vector<unsigned int>(100));

  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      some_huge_vector[i][j] = ((i + 1) * 7 % 19 - 9) * ((j + 1) * 3 % 19 - 9);
      unsigned_int_vector[i][j] = std::abs(some_huge_vector[i][j]);
    }
  }

  // default: max_iteration_count = 16;
  cpp_dump(some_huge_vector);

  std::clog << "\n// manipulator-front-etc.png\n" << std::endl;

  cpp_dump(some_huge_vector | cp::back(10) | cp::both_ends(5) | cp::dec(2));

  std::clog << "\n// manipulator-index.png\n" << std::endl;

  CPP_DUMP_SET_OPTION(max_iteration_count, 5);
  cpp_dump(some_huge_vector | cp::dec(2) | cp::index());

  std::clog << "\n// manipulator-int-style.png\n" << std::endl;

  cpp_dump(0x3e8 | cp::bin(16, 4));
  cpp_dump(0x3e8 | cp::oct(6, 3));
  cpp_dump(0x3e8 | cp::hex(4, 2));
  cpp_dump(0x3e8 | cp::dec(4));

  std::clog << "\n// manipulator-bin-etc.png\n" << std::endl;

  auto &signed_int_vector = some_huge_vector;

  cpp_dump(signed_int_vector | cp::front(2) | cp::hex(2));
  cpp_dump(unsigned_int_vector | cp::front(2) | cp::hex(2));
  cpp_dump(signed_int_vector | cp::front(2) | cp::dec(2));
  cpp_dump(unsigned_int_vector | cp::front(2) | cp::dec(2));

  std::clog << "\n// manipulator-ubin-etc.png\n" << std::endl;

  cpp_dump(signed_int_vector | cp::front(2) | cp::uhex());
  cpp_dump(unsigned_int_vector | cp::front(2) | cp::uhex(2));
  cpp_dump(signed_int_vector | cp::front(2) | cp::udec(2));
  cpp_dump(unsigned_int_vector | cp::front(2) | cp::udec(2));

  std::clog << "\n// manipulator-format.png\n" << std::endl;

  constexpr double pi = M_PI;
  cpp_dump(pi | cp::format("%.10f"));

  std::clog << "\n// manipulator-bw-boolnum.png\n" << std::endl;

  std::vector<std::vector<bool>> bool_vector{
      {true, false, true, true, false},
      {false, false, false, true, true},
  };

  cpp_dump(bool_vector | cp::bw());
  cpp_dump(bool_vector | cp::bw(true));
  cpp_dump(bool_vector | cp::boolnum());

  std::clog << "\n// manipulator-stresc.png\n" << std::endl;

  cpp_dump("\a\t\\\"\r\n\x7f need to be escaped.");
  cpp_dump("\a\t\\\"\r\n\x7f need to be escaped." | cp::stresc());

  std::clog << "\n// manipulator-charhex.png\n" << std::endl;

  for (auto c : "\a\t\\\"\r\n\x7f ABC") cpp_dump(c | cp::charhex());

  std::clog << "\n// manipulator-addr.png\n" << std::endl;

  int my_int = 15;
  int *int_ptr = &my_int;
  int **int_ptr_ptr = &int_ptr;
  cpp_dump(int_ptr_ptr);
  cpp_dump(int_ptr_ptr | cp::addr());
  cpp_dump(int_ptr_ptr | cp::addr(1));

  std::clog << std::endl;
}
