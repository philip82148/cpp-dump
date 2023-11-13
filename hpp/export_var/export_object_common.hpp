/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../utility.hpp"

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1                                                      \
  namespace cpp_dump {                                                                                \
                                                                                                      \
  extern inline std::size_t max_line_width;                                                           \
                                                                                                      \
  extern inline std::size_t max_depth;                                                                \
                                                                                                      \
  namespace _detail {                                                                                 \
                                                                                                      \
  template <typename T>                                                                               \
  std::string                                                                                         \
  export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &); \
                                                                                                      \
  } /* namespace _detail */                                                                           \
                                                                                                      \
  } /* namespace cpp_dump */

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_1                                                 \
  if (current_depth >= max_depth)                                                                  \
    return es::identifier(type_name) + es::bracket("{ ", current_depth) + es::op("...")            \
           + es::bracket(" }", current_depth);                                                     \
                                                                                                   \
  std::string new_indent = indent + "  ";                                                          \
  std::size_t next_depth = current_depth + 1;                                                      \
                                                                                                   \
  bool shift_indent = false;                                                                       \
                                                                                                   \
  std::string output;                                                                              \
  bool is_first;

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_2                                                 \
  auto append_output = [&](const std::string &member_name, const auto &member) -> void {           \
    if (is_first) {                                                                                \
      is_first = false;                                                                            \
    } else {                                                                                       \
      output += es::op(", ");                                                                      \
    }                                                                                              \
                                                                                                   \
    if (shift_indent) {                                                                            \
      output += "\n" + new_indent + es::member(member_name) + es::op("= ");                        \
      output += export_var(                                                                        \
          member, new_indent, get_last_line_length(output), next_depth, false, command             \
      );                                                                                           \
    } else {                                                                                       \
      output += es::member(member_name) + es::op("= ");                                            \
      output += export_var(                                                                        \
          member, indent, last_line_length + get_length(output), next_depth, true, command         \
      );                                                                                           \
    }                                                                                              \
  };

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_3                                                 \
  rollback:                                                                                        \
  output = es::identifier(type_name) + es::bracket("{ ", current_depth);                           \
  is_first = true;

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2                                                   \
  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_1;                                                      \
  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_2;                                                      \
  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2_3;

#define _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON3                                                   \
  if (!shift_indent) {                                                                             \
    output += es::bracket(" }", current_depth);                                                    \
                                                                                                   \
    if (!has_newline(output) && last_line_length + get_length(output) <= max_line_width)           \
      return output;                                                                               \
                                                                                                   \
    if (fail_on_newline) return "\n";                                                              \
                                                                                                   \
    shift_indent = true;                                                                           \
    goto rollback;                                                                                 \
  }                                                                                                \
                                                                                                   \
  output += "\n" + indent + es::bracket("}", current_depth);                                       \
                                                                                                   \
  return output;
