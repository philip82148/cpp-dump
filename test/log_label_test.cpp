#include <array>
#include <iostream>
#include <ostream>
#include <vector>
//
#include "../dump.hpp"

// These are often used to write code faster in competitive programming.
// This is the test for it.
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
using namespace std;
using namespace cpp_dump;

#define PRINT(x) (x), (clog << #x << ";" << endl)

int main(int argc, char *argv[]) {
  if (argc != 2) return 1;
  int es_index = stoi(argv[1]);

  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
  );

  auto vec = vector<vector<int>>{{3, 1, 4}, {1, 5, 9}};

  clog << "// default" << endl;
  cpp_dump(vec);

  clog << "// line()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::line());
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::line(0, true));
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::line(12));
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::line(12, true));
  cpp_dump(vec);

  clog << "// basename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::basename());
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::basename(true));
  cpp_dump(vec);

  clog << "// filename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::filename());
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::filename(true));
  cpp_dump(vec);

  // clog << "// filepath()" << endl;
  // PRINT(cpp_dump::log_label_func = log_label::filepath());
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::filepath(0, true));
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::filepath(10));
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::filepath(10, true));
  // cpp_dump(vec);

  clog << "// fixed_length()" << endl;
  // PRINT(cpp_dump::log_label_func = log_label::fixed_length());
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 0, true));
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::fixed_length(70));
  // cpp_dump(vec);
  // PRINT(cpp_dump::log_label_func = log_label::fixed_length(70, 0, true));
  // cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 20));
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 20, true));
  cpp_dump(vec);
}
