#include <string>

#include "../dump.hpp"

enum class enum_a { s, k, l };

struct class_a {
  int int_a = 5;
  std::string a_str() const { return std::to_string(int_a); }
};

struct class_b {
  int int_b;
  std::string b_str() const { return std::to_string(int_b); }
};

CPP_DUMP_DEFINE_EXPORT_ENUM(enum_a, enum_a::s, enum_a::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(class_a, int_a, a_str());
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(int_b, b_str());
CPP_DUMP_SET_OPTION_GLOBAL(max_line_width, 100);
CPP_DUMP_SET_OPTION_GLOBAL(max_depth, 5);
CPP_DUMP_SET_OPTION_GLOBAL(max_iteration_count, 20);
CPP_DUMP_SET_OPTION_GLOBAL(enable_asterisk, false);
CPP_DUMP_SET_OPTION_GLOBAL(print_expr, true);
CPP_DUMP_SET_OPTION_GLOBAL(log_label_func, cpp_dump::log_label::filename());
CPP_DUMP_SET_OPTION_GLOBAL(es_style, cpp_dump::es_style_t::by_syntax);
CPP_DUMP_SET_OPTION_GLOBAL(
    cont_indent_style, cpp_dump::cont_indent_style_t::when_non_tuples_nested
);

void odr_test();
