#pragma once

#include <string>
#include <type_traits>

#include "iterable_like.hpp"

namespace cpp_dump {

extern inline const int MAX_ITERABLE_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string);

template <const size_t i, const size_t size, typename T>
inline auto __export_tuple_like(T &&value, std::string indent,
                                std::string delimiter)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  if constexpr (i < size - 1) {
    return export_var(std::get<i>(value), indent) + delimiter +
           __export_tuple_like<i + 1, size>(value, indent, delimiter);
  } else {
    return export_var(std::get<i>(value), indent);
  }
}

template <typename T>
inline auto export_tuple_like(T &&value, std::string indent)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string value_string =
      "( " +
      __export_tuple_like<0, std::tuple_size_v<std::remove_cvref_t<T>>>(
          value, indent, ", ") +
      " )";

  if (value_string.find("\n") == std::string::npos &&
      value_string.length() <= MAX_ITERABLE_LINE_WIDTH)
    return value_string;

  std::string new_indent = indent + "  ";
  return "(\n" + new_indent +
         __export_tuple_like<0, std::tuple_size_v<std::remove_cvref_t<T>>>(
             value, new_indent, ",\n" + new_indent) +
         "\n" + indent + ")";
}

template <typename T>
inline auto export_tuple_like(T &value, std::string indent)
    -> std::enable_if_t<is_pair<T>, std::string> {
  return export_tuple_like(std::make_tuple(value.first, value.second), indent);
}

}  // namespace cpp_dump
