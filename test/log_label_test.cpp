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

#define PRINT(x) (x), (clog << #x ";" << endl)

int main(int argc, char *argv[]) {
  if (argc != 3) return 1;
  bool path_dependent = static_cast<bool>(stoi(argv[1]));
  bool es_index = static_cast<bool>(stoi(argv[2]));

  CPP_DUMP_SET_OPTION(
      es_style, (array{cpp_dump::es_style_t::no_es, cpp_dump::es_style_t::by_syntax}[es_index])
  );

  auto vec = vector<vector<int>>{{3, 1, 4}, {1, 5, 9}};

  clog << "// default" << endl;
  cpp_dump(vec);

  clog << "// line()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::line());
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::line(true));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  clog << "// basename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::basename());
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::basename(true));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  clog << "// filename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::filename());
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::filename(true));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  if (path_dependent) {
    clog << "// fullpath()" << endl;
    PRINT(cpp_dump::log_label_func = log_label::fullpath(0));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fullpath(0, false, 33));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fullpath(0, true, 33));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);

    clog << "// fixed_length()" << endl;
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 0, 0));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 0, 0, true));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 0, 33));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 0, 33, true));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 40, 33, false));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 40, 33, true));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
  }

  clog << "// line()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::line(false, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::line(true, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  clog << "// basename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::basename(false, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::basename(true, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  clog << "// filename()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::filename(false, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  PRINT(cpp_dump::log_label_func = log_label::filename(true, 40));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);

  if (path_dependent) {
    clog << "// fullpath()" << endl;
    PRINT(cpp_dump::log_label_func = log_label::fullpath(33, false, 40));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fullpath(33, true, 40));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
  }

  clog << "// fixed_length()" << endl;
  PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 40, 0));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
  if (path_dependent) {
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(40, 50, 33, false));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
    PRINT(cpp_dump::log_label_func = log_label::fixed_length(40, 50, 33, true));
    cpp_dump(vec);
    cpp_dump(vec);
    cpp_dump(vec);
  }
  PRINT(cpp_dump::log_label_func = log_label::fixed_length(0, 40, 0, true));
  cpp_dump(vec);
  cpp_dump(vec);
  cpp_dump(vec);
}
