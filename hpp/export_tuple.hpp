/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <tuple>
#include <type_traits>

#include "./escape_sequence.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

extern inline size_t max_line_width;

extern inline size_t max_depth;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_in_one_line(
    const T &tuple, const std::string &indent, size_t last_line_length, size_t next_depth
) -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(tuple), indent, last_line_length, next_depth, true);
  if (has_newline(output)) return "\n";

  if constexpr (i < size - 1) {
    return output + ", "
           + _export_tuple_in_one_line<i + 1, size>(tuple, indent, output.length() + 2, next_depth);
  } else {
    return output;
  }
}

template <const size_t i, const size_t size, typename T>
inline auto _export_tuple_in_lines(const T &tuple, const std::string &indent, size_t next_depth)
    -> std::enable_if_t<is_tuple<T>, std::string> {
  std::string output = export_var(std::get<i>(tuple), indent, indent.length(), next_depth, false);

  if constexpr (i < size - 1) {
    return output + ",\n" + indent + _export_tuple_in_lines<i + 1, size>(tuple, indent, next_depth);
  } else {
    return output;
  }
}

template <typename T>
inline auto export_tuple(
    const T &tuple,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_tuple<T>, std::string> {
  constexpr size_t tuple_size = std::tuple_size_v<T>;

  if constexpr (tuple_size == 0) {
    return with_es::bracket("( )", current_depth);
  } else {
    if (current_depth >= max_depth)
      return with_es::bracket("( ", current_depth) + "..." + with_es::bracket(" )", current_depth);

    size_t next_depth = current_depth + 1;

    std::string output =
        with_es::bracket("( ", current_depth)
        + _export_tuple_in_one_line<0, tuple_size>(tuple, indent, last_line_length + 2, next_depth)
        + with_es::bracket(" )", current_depth);

    if (!has_newline(output) && output.length() <= max_line_width) return output;

    if (fail_on_newline) return "\n";

    std::string new_indent = indent + "  ";
    return with_es::bracket("(\n", current_depth) + new_indent
           + _export_tuple_in_lines<0, tuple_size>(tuple, new_indent, next_depth) + "\n" + indent
           + with_es::bracket(")", current_depth);
  }
}

}  // namespace _detail

}  // namespace cpp_dump
