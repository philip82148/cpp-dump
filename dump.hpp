#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "export_var.hpp"
#include "utility.hpp"

#define EXPAND_VA_ARGS1_(arg1) #arg1, (arg1)
#define EXPAND_VA_ARGS2_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS1_(__VA_ARGS__)
#define EXPAND_VA_ARGS3_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS2_(__VA_ARGS__)
#define EXPAND_VA_ARGS4_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS3_(__VA_ARGS__)
#define EXPAND_VA_ARGS5_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS4_(__VA_ARGS__)
#define EXPAND_VA_ARGS6_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS5_(__VA_ARGS__)
#define EXPAND_VA_ARGS7_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS6_(__VA_ARGS__)
#define EXPAND_VA_ARGS8_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS7_(__VA_ARGS__)
#define EXPAND_VA_ARGS9_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS8_(__VA_ARGS__)
#define EXPAND_VA_ARGS10_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS9_(__VA_ARGS__)
#define EXPAND_VA_ARGS11_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS10_(__VA_ARGS__)
#define EXPAND_VA_ARGS12_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS11_(__VA_ARGS__)

#define VA_ARGS_SIZE_AUX_(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, \
                          arg12, size, ...)                                                   \
  size
#define VA_ARGS_SIZE_(...) VA_ARGS_SIZE_AUX_(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define GET_EXPAND_VA_ARGS_MACRO_(size) EXPAND_VA_ARGS##size##_
#define EXPAND_VA_ARGS_(size, ...) GET_EXPAND_VA_ARGS_MACRO_(size)(__VA_ARGS__)

#define dump(...) cpp_dump::_dump(EXPAND_VA_ARGS_(VA_ARGS_SIZE_(__VA_ARGS__), __VA_ARGS__))

namespace cpp_dump {

inline const int max_line_width = 80;

template <typename T>
size_t _dump_recursive_aux(size_t last_line_length, std::string expr, T &&value) {
  const std::string indent11 = "           ";

  if (_has_lf(expr) && last_line_length > 9) {
    std::clog << "\n" << indent11;
    last_line_length = 11;
  }

  struct prefix_and_value_string {
    std::string prefix;
    std::string value_string;
    bool over_max_line_width;
    bool value_string_has_lf;

    prefix_and_value_string(std::string prefix, std::string value_string, bool over_max_width,
                            bool value_string_has_lf)
        : prefix(prefix),
          value_string(value_string),
          over_max_line_width(over_max_width),
          value_string_has_lf(value_string_has_lf) {}
  };

  auto get_prefix_and_value_string = [=, &value](std::string prefix) -> prefix_and_value_string {
    auto new_last_line_length = _last_line_length(prefix, last_line_length);

    std::string value_string = export_var(value, indent11, new_last_line_length);

    bool over_max_line_width =
        new_last_line_length + _first_line_length(value_string) > max_line_width;

    bool value_string_has_lf = _has_lf(value_string);

    return {prefix, value_string, over_max_line_width, value_string_has_lf};
  };

  auto output = [=](prefix_and_value_string &pattern) -> size_t {
    std::string output = pattern.prefix + pattern.value_string;
    std::clog << output;

    return _last_line_length(output, last_line_length);
  };

  prefix_and_value_string pattern1 = get_prefix_and_value_string(expr + " => ");
  if (!pattern1.over_max_line_width) {
    if (pattern1.value_string_has_lf) {
      if (last_line_length <= 11) {
        prefix_and_value_string pattern3b =
            get_prefix_and_value_string(expr + "\n" + indent11 + "=> ");

        if (!pattern3b.value_string_has_lf) return output(pattern3b);
      } else {
        prefix_and_value_string pattern2 =
            get_prefix_and_value_string("\n" + indent11 + expr + " => ");

        if (!pattern2.value_string_has_lf) return output(pattern2);

        prefix_and_value_string pattern3a =
            get_prefix_and_value_string("\n" + indent11 + expr + "\n" + indent11 + "=> ");

        if (!pattern3a.value_string_has_lf) return output(pattern3a);
      }
    }

    return output(pattern1);
  }

  if (last_line_length <= 11) {
    prefix_and_value_string pattern3b = get_prefix_and_value_string(expr + "\n" + indent11 + "=> ");

    return output(pattern3b);
  }

  prefix_and_value_string pattern2 = get_prefix_and_value_string("\n" + indent11 + expr + " => ");
  if (!pattern2.over_max_line_width) {
    if (pattern2.value_string_has_lf) {
      prefix_and_value_string pattern3a =
          get_prefix_and_value_string("\n" + indent11 + expr + "\n" + indent11 + "=> ");

      if (!pattern3a.value_string_has_lf) return output(pattern3a);
    }

    return output(pattern2);
  }

  prefix_and_value_string pattern3a =
      get_prefix_and_value_string("\n" + indent11 + expr + "\n" + indent11 + "=> ");

  return output(pattern3a);
}

inline void _dump_recursive(size_t) {}

template <typename T, typename... Args>
inline void _dump_recursive(size_t last_line_length, std::string expr, T &&value, Args &&...args) {
  std::clog << ", ";
  last_line_length += 2;

  last_line_length = _dump_recursive_aux(last_line_length, expr, value);

  _dump_recursive(last_line_length, args...);
}

template <typename T, typename... Args>
void _dump(std::string expr, T &&value, Args &&...args) {
  std::clog << "[dump()] ";

  auto last_line_length = _dump_recursive_aux(9, expr, value);

  _dump_recursive(last_line_length, args...);
  std::clog << std::endl;
}

}  // namespace cpp_dump
