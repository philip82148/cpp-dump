#pragma once

#include <string>
#include <type_traits>

#include "type_check.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline const int max_line_width;

template <typename T>
std::string export_var(T &&, std::string, size_t, bool);

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_like_in_one_line(T &&value, std::string indent, size_t last_line_length)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(value), indent, last_line_length, true);
  if (_has_newline(output)) return "\n";

  if constexpr (i < size - 1) {
    return output + ", " +
           _export_tuple_like_in_one_line<i + 1, size>(value, indent, output.length() + 2);
  } else {
    return output;
  }
}

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_like_in_lines(T &&value, std::string indent)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(value), indent, indent.length(), false);

  if constexpr (i < size - 1) {
    return output + ",\n" + indent + _export_tuple_like_in_lines<i + 1, size>(value, indent);
  } else {
    return output;
  }
}

template <typename T>
inline auto export_tuple_like(T &&value, std::string indent, size_t last_line_length,
                              bool fail_on_newline) -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string value_string =
      "( " +
      _export_tuple_like_in_one_line<0, std::tuple_size_v<std::remove_cvref_t<T>>>(
          value, indent, last_line_length + 2) +
      " )";

  if (!_has_newline(value_string) && value_string.length() <= max_line_width) return value_string;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  return "(\n" + new_indent +
         _export_tuple_like_in_lines<0, std::tuple_size_v<std::remove_cvref_t<T>>>(value,
                                                                                   new_indent) +
         "\n" + indent + ")";
}

template <typename T>
inline auto export_tuple_like(T &&value, std::string indent, size_t last_line_length,
                              bool fail_on_newline) -> std::enable_if_t<is_pair<T>, std::string> {
  return export_tuple_like(std::make_tuple(value.first, value.second), indent, last_line_length,
                           fail_on_newline);
}

}  // namespace cpp_dump
