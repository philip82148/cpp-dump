/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <queue>
#include <stack>
#include <string>
#include <type_traits>

#include "./escape_sequence.hpp"
#include "./export_command.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

extern inline size_t max_line_width;

extern inline size_t max_depth;

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, size_t, size_t, bool, const export_command &);

template <typename T, typename U>
inline std::string _export_xixo_common(
    const T &xixo,
    const U &member,
    const std::string &class_name,
    const std::string &member_name,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  if (xixo.empty())
    return es::identifier(class_name) + es::bracket("{ ", current_depth) + es::member("size()")
           + es::op("= ") + es::number("0") + es::bracket(" }", current_depth);

  if (current_depth >= max_depth)
    return es::identifier(class_name) + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  size_t next_depth = current_depth + 1;

  std::string prefix = es::identifier(class_name) + es::bracket("{ ", current_depth)
                       + es::member(member_name) + es::op("= ");
  std::string output =
      prefix
      + export_var(member, indent, last_line_length + get_length(prefix), next_depth, true, command)
      + es::op(", ") + es::member("size()") + es::op("= ") + es::number(std::to_string(xixo.size()))
      + es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + es::member(member_name) + es::op("= ");
  output = es::identifier(class_name) + es::bracket("{\n", current_depth) + prefix
           + export_var(member, new_indent, get_length(prefix), next_depth, false, command)
           + es::op(",\n") + new_indent + es::member("size()") + es::op("= ")
           + es::number(std::to_string(xixo.size())) + "\n" + indent
           + es::bracket("}", current_depth);

  return output;
}

template <typename... Args>
inline std::string export_xixo(
    const std::queue<Args...> &queue,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return _export_xixo_common(
      queue,
      queue.front(),
      "std::queue",
      "front()",
      indent,
      last_line_length,
      current_depth,
      fail_on_newline,
      command
  );
}

template <typename... Args>
inline std::string export_xixo(
    const std::priority_queue<Args...> &pq,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return _export_xixo_common(
      pq,
      pq.top(),
      "std::priority_queue",
      "top()",
      indent,
      last_line_length,
      current_depth,
      fail_on_newline,
      command
  );
}

template <typename... Args>
inline std::string export_xixo(
    const std::stack<Args...> &stack,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return _export_xixo_common(
      stack,
      stack.top(),
      "std::stack",
      "top()",
      indent,
      last_line_length,
      current_depth,
      fail_on_newline,
      command
  );
}

}  // namespace _detail

}  // namespace cpp_dump
