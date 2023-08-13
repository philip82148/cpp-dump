#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "export_var.hpp"

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

inline const int MAX_ITERABLE_LINE_WIDTH = 40;

inline auto __first_line_length(std::string value) {
  auto lf_pos = value.find("\n");

  if (lf_pos == std::string::npos) return value.length();

  return lf_pos;
}

inline auto __last_line_length(std::string value,
                               int additional_first_line_length = 0) {
  auto lf_pos = value.rfind("\n");

  if (lf_pos == std::string::npos)
    return additional_first_line_length + value.length();

  return value.length() - lf_pos - 1;
}

using __last_line_length_type = decltype(__last_line_length(""));

inline __last_line_length_type __dump_value_string(
    __last_line_length_type last_line_length, std::string expr,
    std::string value_string) {
  std::string output = expr + " => " + value_string;
  auto output_first_line_length = __first_line_length(output);

  if (last_line_length + output_first_line_length <= MAX_LINE_WIDTH) {
    std::clog << output;

    return __last_line_length(output, last_line_length);
  }

  if (last_line_length > 9) {
    std::clog << "\n         ";

    if (9 + output_first_line_length <= MAX_LINE_WIDTH) {
      std::clog << output;

      return __last_line_length(output, 9);
    }
  }

  std::clog << expr << "\n           => " << value_string;

  return __last_line_length(value_string, 14);
}

inline void __dump_aux(__last_line_length_type) {}

template <typename T, typename... Args>
void __dump_aux(__last_line_length_type last_line_length, std::string expr,
                T &&value, Args &&...args) {
  std::clog << ", ";
  last_line_length += 2;

  std::string value_string = export_var(value, "         ");
  last_line_length = __dump_value_string(last_line_length, expr, value_string);

  __dump_aux(last_line_length, args...);
}

template <typename T, typename... Args>
void __dump(std::string expr, T &&value, Args &&...args) {
  std::clog << "[dump()] ";

  std::string value_string = export_var(value, "         ");
  auto last_line_length = __dump_value_string(9, expr, value_string);

  __dump_aux(last_line_length, args...);
  std::clog << std::endl;
}

}  // namespace cpp_dump
