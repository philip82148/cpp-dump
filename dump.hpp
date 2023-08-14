#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "export_var.hpp"
#include "utility.hpp"

#define __expand1(arg1) #arg1, (arg1)
#define __expand2(arg1, ...) __expand1(arg1), __expand1(__VA_ARGS__)
#define __expand3(arg1, ...) __expand1(arg1), __expand2(__VA_ARGS__)
#define __expand4(arg1, ...) __expand1(arg1), __expand3(__VA_ARGS__)
#define __expand5(arg1, ...) __expand1(arg1), __expand4(__VA_ARGS__)
#define __expand6(arg1, ...) __expand1(arg1), __expand5(__VA_ARGS__)
#define __expand7(arg1, ...) __expand1(arg1), __expand6(__VA_ARGS__)
#define __expand8(arg1, ...) __expand1(arg1), __expand7(__VA_ARGS__)
#define __expand9(arg1, ...) __expand1(arg1), __expand8(__VA_ARGS__)
#define __expand10(arg1, ...) __expand1(arg1), __expand9(__VA_ARGS__)
#define __expand11(arg1, ...) __expand1(arg1), __expand10(__VA_ARGS__)
#define __expand12(arg1, ...) __expand1(arg1), __expand11(__VA_ARGS__)

#define __va_args_size_aux(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, \
                           arg9, arg10, arg11, arg12, size, ...)           \
  size
#define __va_args_size(...) \
  __va_args_size_aux(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define __get_expand_macro(size) __expand##size
#define __expand(size, ...) __get_expand_macro(size)(__VA_ARGS__)

#define dump(...) \
  cpp_dump::__dump(__expand(__va_args_size(__VA_ARGS__), __VA_ARGS__))

namespace cpp_dump {

inline const int MAX_LINE_WIDTH = 80;

template <typename T>
size_t __dump_recursive_aux(size_t last_line_length, std::string expr,
                            T &&value) {
  const std::string indent11 = "           ";

  if (__has_lf(expr) && last_line_length > 9) {
    std::clog << "\n" << indent11;
    last_line_length = 11;
  }

  struct prefix_and_value_string {
    std::string prefix;
    std::string value_string;
    bool over_max_line_width;
    bool value_string_has_lf;

    prefix_and_value_string(std::string prefix, std::string value_string,
                            bool over_max_width, bool value_string_has_lf)
        : prefix(prefix),
          value_string(value_string),
          over_max_line_width(over_max_width),
          value_string_has_lf(value_string_has_lf) {}
  };

  auto get_prefix_and_value_string =
      [=, &value](std::string prefix) -> prefix_and_value_string {
    auto new_last_line_length = __last_line_length(prefix, last_line_length);

    std::string value_string =
        export_var(value, indent11, new_last_line_length);

    bool over_max_line_width =
        new_last_line_length + __first_line_length(value_string) >
        MAX_LINE_WIDTH;

    bool value_string_has_lf = __has_lf(value_string);

    return {prefix, value_string, over_max_line_width, value_string_has_lf};
  };

  auto output = [=](prefix_and_value_string &pattern) -> size_t {
    std::string output = pattern.prefix + pattern.value_string;
    std::clog << output;

    return __last_line_length(output, last_line_length);
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

        prefix_and_value_string pattern3a = get_prefix_and_value_string(
            "\n" + indent11 + expr + "\n" + indent11 + "=> ");

        if (!pattern3a.value_string_has_lf) return output(pattern3a);
      }
    }

    return output(pattern1);
  }

  if (last_line_length <= 11) {
    prefix_and_value_string pattern3b =
        get_prefix_and_value_string(expr + "\n" + indent11 + "=> ");

    return output(pattern3b);
  }

  prefix_and_value_string pattern2 =
      get_prefix_and_value_string("\n" + indent11 + expr + " => ");
  if (!pattern2.over_max_line_width) {
    if (pattern2.value_string_has_lf) {
      prefix_and_value_string pattern3a = get_prefix_and_value_string(
          "\n" + indent11 + expr + "\n" + indent11 + "=> ");

      if (!pattern3a.value_string_has_lf) return output(pattern3a);
    }

    return output(pattern2);
  }

  prefix_and_value_string pattern3a = get_prefix_and_value_string(
      "\n" + indent11 + expr + "\n" + indent11 + "=> ");

  return output(pattern3a);
}

inline void __dump_recursive(size_t) {}

template <typename T, typename... Args>
inline void __dump_recursive(size_t last_line_length, std::string expr,
                             T &&value, Args &&...args) {
  std::clog << ", ";
  last_line_length += 2;

  last_line_length = __dump_recursive_aux(last_line_length, expr, value);

  __dump_recursive(last_line_length, args...);
}

template <typename T, typename... Args>
void __dump(std::string expr, T &&value, Args &&...args) {
  std::clog << "[dump()] ";

  auto last_line_length = __dump_recursive_aux(9, expr, value);

  __dump_recursive(last_line_length, args...);
  std::clog << std::endl;
}

}  // namespace cpp_dump
