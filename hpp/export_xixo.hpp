/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <queue>
#include <stack>
#include <string>
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

template <typename... Args>
inline std::string export_xixo(
    const std::queue<Args...> &queue,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  if (queue.empty())
    return with_es::identifier("std::queue") + with_es::bracket("{ ", current_depth)
           + with_es::member("size()") + with_es::op("= ") + with_es::number("0")
           + with_es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return with_es::identifier("std::queue") + with_es::bracket("{ ", current_depth)
           + with_es::op("...") + with_es::bracket(" }", current_depth);

  size_t next_depth = current_depth + 1;

  std::string prefix = with_es::identifier("std::queue") + with_es::bracket("{ ", current_depth)
                       + with_es::member("front()") + with_es::op("= ");
  std::string output =
      prefix
      + export_var(queue.front(), indent, last_line_length + get_length(prefix), next_depth, true)
      + with_es::op(", ") + with_es::member("size()") + with_es::op("= ")
      + with_es::number(std::to_string(queue.size())) + with_es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + with_es::member("front()") + with_es::op("= ");
  output = with_es::identifier("std::queue") + with_es::bracket("{\n", current_depth) + prefix
           + export_var(queue.front(), new_indent, get_length(prefix), next_depth, false)
           + with_es::op(",\n") + new_indent + with_es::member("size()") + with_es::op("= ")
           + with_es::number(std::to_string(queue.size())) + "\n" + indent
           + with_es::bracket("}", current_depth);

  return output;
}

template <typename... Args>
inline std::string export_xixo(
    const std::priority_queue<Args...> &pq,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  if (pq.empty())
    return with_es::identifier("std::priority_queue") + with_es::bracket("{ ", current_depth)
           + with_es::member("size()") + "= " + with_es::number("0")
           + with_es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return with_es::identifier("std::priority_queue") + with_es::bracket("{ ", current_depth)
           + with_es::op("...") + with_es::bracket(" }", current_depth);

  size_t next_depth = current_depth + 1;

  std::string prefix = with_es::identifier("std::priority_queue")
                       + with_es::bracket("{ ", current_depth) + with_es::member("top()")
                       + with_es::op("= ");
  std::string output =
      prefix + export_var(pq.top(), indent, last_line_length + get_length(prefix), next_depth, true)
      + with_es::op(", ") + with_es::member("size()") + with_es::op("= ")
      + with_es::number(std::to_string(pq.size())) + with_es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + with_es::member("top()") + with_es::op("= ");
  output = with_es::identifier("std::priority_queue") + with_es::bracket("{\n", current_depth)
           + prefix + export_var(pq.top(), new_indent, get_length(prefix), next_depth, false)
           + with_es::op(",\n") + new_indent + with_es::member("size()") + with_es::op("= ")
           + with_es::number(std::to_string(pq.size())) + "\n" + indent
           + with_es::bracket("}", current_depth);

  return output;
}

template <typename... Args>
inline std::string export_xixo(
    const std::stack<Args...> &stack,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  if (stack.empty())
    return with_es::identifier("std::stack") + with_es::bracket("{ ", current_depth)
           + with_es::member("size()") + with_es::op("= ") + with_es::number("0")
           + with_es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return with_es::identifier("std::stack") + with_es::bracket("{ ", current_depth)
           + with_es::op("...") + with_es::bracket(" }", current_depth);

  size_t next_depth = current_depth + 1;

  std::string prefix = with_es::identifier("std::stack") + with_es::bracket("{ ", current_depth)
                       + with_es::member("top()") + with_es::op("= ");
  std::string output =
      prefix
      + export_var(stack.top(), indent, last_line_length + get_length(prefix), next_depth, true)
      + with_es::op(", ") + with_es::member("size()") + with_es::op("= ")
      + with_es::number(std::to_string(stack.size())) + with_es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + with_es::member("top()") + with_es::op("= ");
  output = with_es::identifier("std::stack") + with_es::bracket("{\n", current_depth) + prefix
           + export_var(stack.top(), new_indent, get_length(prefix), next_depth, false)
           + with_es::op(",\n") + new_indent + with_es::member("size()") + with_es::op("= ")
           + with_es::number(std::to_string(stack.size())) + "\n" + indent
           + with_es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
