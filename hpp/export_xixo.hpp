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

extern inline std::size_t max_line_width;

extern inline std::size_t max_depth;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, std::size_t, std::size_t, bool);

template <typename... Args>
inline std::string export_xixo(
    const std::queue<Args...> &queue,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline
) {
  if (queue.empty())
    return es::identifier("std::queue") + es::bracket("{ ", current_depth) + es::member("size()")
           + es::op("= ") + es::number("0") + es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return es::identifier("std::queue") + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  std::size_t next_depth = current_depth + 1;

  std::string prefix = es::identifier("std::queue") + es::bracket("{ ", current_depth)
                       + es::member("front()") + es::op("= ");
  std::string output =
      prefix
      + export_var(queue.front(), indent, last_line_length + get_length(prefix), next_depth, true)
      + es::op(", ") + es::member("size()") + es::op("= ")
      + es::number(std::to_string(queue.size())) + es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + es::member("front()") + es::op("= ");
  output = es::identifier("std::queue") + es::bracket("{\n", current_depth) + prefix
           + export_var(queue.front(), new_indent, get_length(prefix), next_depth, false)
           + es::op(",\n") + new_indent + es::member("size()") + es::op("= ")
           + es::number(std::to_string(queue.size())) + "\n" + indent
           + es::bracket("}", current_depth);

  return output;
}

template <typename... Args>
inline std::string export_xixo(
    const std::priority_queue<Args...> &pq,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline
) {
  if (pq.empty())
    return es::identifier("std::priority_queue") + es::bracket("{ ", current_depth)
           + es::member("size()") + "= " + es::number("0") + es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return es::identifier("std::priority_queue") + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  std::size_t next_depth = current_depth + 1;

  std::string prefix = es::identifier("std::priority_queue") + es::bracket("{ ", current_depth)
                       + es::member("top()") + es::op("= ");
  std::string output =
      prefix + export_var(pq.top(), indent, last_line_length + get_length(prefix), next_depth, true)
      + es::op(", ") + es::member("size()") + es::op("= ") + es::number(std::to_string(pq.size()))
      + es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + es::member("top()") + es::op("= ");
  output = es::identifier("std::priority_queue") + es::bracket("{\n", current_depth) + prefix
           + export_var(pq.top(), new_indent, get_length(prefix), next_depth, false) + es::op(",\n")
           + new_indent + es::member("size()") + es::op("= ")
           + es::number(std::to_string(pq.size())) + "\n" + indent
           + es::bracket("}", current_depth);

  return output;
}

template <typename... Args>
inline std::string export_xixo(
    const std::stack<Args...> &stack,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline
) {
  if (stack.empty())
    return es::identifier("std::stack") + es::bracket("{ ", current_depth) + es::member("size()")
           + es::op("= ") + es::number("0") + es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return es::identifier("std::stack") + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  std::size_t next_depth = current_depth + 1;

  std::string prefix = es::identifier("std::stack") + es::bracket("{ ", current_depth)
                       + es::member("top()") + es::op("= ");
  std::string output =
      prefix
      + export_var(stack.top(), indent, last_line_length + get_length(prefix), next_depth, true)
      + es::op(", ") + es::member("size()") + es::op("= ")
      + es::number(std::to_string(stack.size())) + es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + es::member("top()") + es::op("= ");
  output = es::identifier("std::stack") + es::bracket("{\n", current_depth) + prefix
           + export_var(stack.top(), new_indent, get_length(prefix), next_depth, false)
           + es::op(",\n") + new_indent + es::member("size()") + es::op("= ")
           + es::number(std::to_string(stack.size())) + "\n" + indent
           + es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
