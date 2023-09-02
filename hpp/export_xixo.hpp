/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

extern inline size_t max_line_width;

extern inline size_t max_depth;

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
inline auto export_xixo(const T &value, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (value.empty()) return "std::queue{ size= 0 }";

  if (current_depth >= max_depth) return "std::queue{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::queue{ front= ";
  std::string output =
      prefix +
      export_var(value.front(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "front= ";
  output = "std::queue{\n" + prefix +
           export_var(value.front(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
inline auto export_xixo(const T &value, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (value.empty()) return "std::priority_queue{ size= 0 }";

  if (current_depth >= max_depth) return "std::priority_queue{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::priority_queue{ top= ";
  std::string output =
      prefix +
      export_var(value.top(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::priority_queue{\n" + prefix +
           export_var(value.top(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
inline auto export_xixo(const T &value, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (value.empty()) return "std::stack{ size= 0 }";

  if (current_depth >= max_depth) return "std::stack{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::stack{ top= ";
  std::string output =
      prefix +
      export_var(value.top(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::stack{\n" + prefix +
           export_var(value.top(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

}  // namespace cpp_dump
