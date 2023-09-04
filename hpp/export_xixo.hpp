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

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
inline auto export_xixo(const T &queue, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (queue.empty()) return "std::queue{ size= 0 }";

  if (current_depth >= max_depth) return "std::queue{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::queue{ front= ";
  std::string output =
      prefix +
      export_var(queue.front(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(queue.size()) + " }";

  if (!has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "front= ";
  output = "std::queue{\n" + prefix +
           export_var(queue.front(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(queue.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
inline auto export_xixo(const T &pq, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (pq.empty()) return "std::priority_queue{ size= 0 }";

  if (current_depth >= max_depth) return "std::priority_queue{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::priority_queue{ top= ";
  std::string output =
      prefix + export_var(pq.top(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(pq.size()) + " }";

  if (!has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::priority_queue{\n" + prefix +
           export_var(pq.top(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(pq.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
inline auto export_xixo(const T &stack, const std::string &indent, size_t last_line_length,
                        size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (stack.empty()) return "std::stack{ size= 0 }";

  if (current_depth >= max_depth) return "std::stack{ ... }";

  size_t next_depth = current_depth + 1;

  std::string prefix = "std::stack{ top= ";
  std::string output =
      prefix +
      export_var(stack.top(), indent, last_line_length + prefix.length(), next_depth, true) +
      ", size= " + std::to_string(stack.size()) + " }";

  if (!has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::stack{\n" + prefix +
           export_var(stack.top(), new_indent, prefix.length(), next_depth, false) + ",\n" +
           new_indent + "size= " + std::to_string(stack.size()) + "\n" + indent + "}";

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
