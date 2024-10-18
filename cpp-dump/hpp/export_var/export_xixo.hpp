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

#include "../class_name.hpp"
#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "./export_object_common.hpp"

namespace cpp_dump {

namespace _detail {

template <typename... Args>
inline std::string export_xixo(
    const std::queue<Args...> &queue,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  std::string class_name = style_class_name("std::queue");

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;

  append_output("size()", queue.size());
  if (!queue.empty()) {
    append_output("front()", queue.front());
    if (queue.size() >= 2) append_output("back()", queue.back());
  }

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}

template <typename... Args>
inline std::string export_xixo(
    const std::priority_queue<Args...> &pq,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  std::string class_name = style_class_name("std::priority_queue");

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;

  append_output("size()", pq.size());
  if (!pq.empty()) append_output("top()", pq.top());

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}

template <typename... Args>
inline std::string export_xixo(
    const std::stack<Args...> &stack,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  std::string class_name = style_class_name("std::stack");

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;

  append_output("size()", stack.size());
  if (!stack.empty()) append_output("top()", stack.top());

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}

}  // namespace _detail

}  // namespace cpp_dump
