#pragma once

#include <string>
#include <type_traits>

#include "type_check.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline const int max_line_width;

template <typename T>
std::string export_var(T &&, std::string, size_t);

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_like_in_one_line(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(value), indent, first_line_length);
  if (_has_lf(output)) return "\n";

  if constexpr (i < size - 1) {
    output += ", ";
    return output + _export_tuple_like_in_one_line<i + 1, size>(value, indent, output.length());
  } else {
    return output;
  }
}

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_like_in_lines(T &&value, std::string indent)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(value), indent);

  if constexpr (i < size - 1) {
    return output + "\n" + indent + _export_tuple_like_in_lines<i + 1, size>(value, indent);
  } else {
    return output;
  }
}

template <typename T>
inline auto export_tuple_like(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string value_string =
      "( " +
      _export_tuple_like_in_one_line<0, std::tuple_size_v<std::remove_cvref_t<T>>>(
          value, indent, first_line_length + 2) +
      " )";

  if (!_has_lf(value_string) && value_string.length() <= max_line_width) return value_string;

  std::string new_indent = indent + "  ";
  return "(\n" + new_indent +
         _export_tuple_like_in_lines<0, std::tuple_size_v<std::remove_cvref_t<T>>>(value,
                                                                                   new_indent) +
         "\n" + indent + ")";
}

template <typename T>
inline auto export_tuple_like(T &value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_pair<T>, std::string> {
  return export_tuple_like(std::make_tuple(value.first, value.second), indent, first_line_length);
}

}  // namespace cpp_dump
