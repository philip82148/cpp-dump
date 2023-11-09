/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"

namespace cpp_dump {

extern inline std::size_t max_line_width;

namespace _detail {

template <typename T>
std::string export_var(
    const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &command
);

template <typename T>
inline auto export_exception(
    const T &exception,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_exception<T>, std::string> {
  std::size_t next_depth = current_depth + 1;

  std::string type_name = get_typename<T>();
  std::string prefix = es::identifier(type_name) + es::bracket("{ ", current_depth)
                       + es::member("what()") + es::op("= ");
  std::string output =
      prefix
      + export_var(
          exception.what(), indent, last_line_length + get_length(prefix), next_depth, true, command
      )
      + es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + es::member("what()") + es::op("= ");
  output =
      es::identifier(type_name) + es::bracket("{\n", current_depth) + prefix
      + export_var(exception.what(), new_indent, get_length(prefix), next_depth, false, command)
      + "\n" + indent + es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
