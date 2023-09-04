/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

#include "./expand_va_macro.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

#define CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_(member) append_output(#member, value.member)
#define CPP_DUMP_DEFINE_EXPORT_OBJECT(type, ...)                                                   \
  namespace cpp_dump {                                                                             \
                                                                                                   \
  extern inline size_t max_line_width;                                                             \
                                                                                                   \
  extern inline size_t max_depth;                                                                  \
                                                                                                   \
  namespace _detail {                                                                              \
                                                                                                   \
  template <>                                                                                      \
  inline constexpr bool _is_exportable_object<type> = true;                                        \
                                                                                                   \
  template <typename T>                                                                            \
  std::string export_var(const T &, const std::string &, size_t, size_t, bool);                    \
                                                                                                   \
  template <>                                                                                      \
  inline std::string export_object(const type &value, const std::string &indent,                   \
                                   size_t last_line_length, size_t current_depth,                  \
                                   bool fail_on_newline) {                                         \
    if (current_depth >= max_depth) return #type "{ ... }";                                        \
                                                                                                   \
    std::string new_indent = indent + "  ";                                                        \
    size_t next_depth = current_depth + 1;                                                         \
                                                                                                   \
    bool shift_indent = false;                                                                     \
                                                                                                   \
    std::string output;                                                                            \
    bool is_first;                                                                                 \
                                                                                                   \
    auto append_output = [&](const std::string &member_name, const auto &member) -> void {         \
      if (is_first) {                                                                              \
        is_first = false;                                                                          \
      } else {                                                                                     \
        output += ", ";                                                                            \
      }                                                                                            \
                                                                                                   \
      if (shift_indent) {                                                                          \
        output += "\n" + new_indent + member_name + "= ";                                          \
        output += export_var(member, new_indent, get_last_line_length(output), next_depth, false); \
      } else {                                                                                     \
        output += member_name + "= ";                                                              \
        output +=                                                                                  \
            export_var(member, indent, last_line_length + output.length(), next_depth, true);      \
      }                                                                                            \
    };                                                                                             \
                                                                                                   \
  rollback:                                                                                        \
    output = #type "{ ";                                                                           \
    is_first = true;                                                                               \
                                                                                                   \
    CPP_DUMP_EXPAND_VA_(CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_, __VA_ARGS__);                          \
                                                                                                   \
    if (!shift_indent) {                                                                           \
      output += " }";                                                                              \
                                                                                                   \
      if (!has_newline(output) && last_line_length + output.length() <= max_line_width)            \
        return output;                                                                             \
                                                                                                   \
      if (fail_on_newline) return "\n";                                                            \
                                                                                                   \
      shift_indent = true;                                                                         \
      goto rollback;                                                                               \
    }                                                                                              \
                                                                                                   \
    output += "\n" + indent + "}";                                                                 \
                                                                                                   \
    return output;                                                                                 \
  }                                                                                                \
                                                                                                   \
  } /* namespace _detail */                                                                        \
                                                                                                   \
  }  // namespace cpp_dump

namespace cpp_dump {

namespace _detail {

template <typename T>
inline std::string export_object(const T &, const std::string &, size_t, size_t, bool);

}  // namespace _detail

}  // namespace cpp_dump
