/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./expand_va_macro.hpp"
#include "./utility.hpp"

#define CPP_DUMP_APPEND_OUTPUT_(property) append_output(#property, value.property)
#define CPP_DUMP_DEFINE_EXPORT_OBJECT(type, ...)                                                  \
  namespace cpp_dump {                                                                            \
                                                                                                  \
  extern inline size_t max_line_width;                                                            \
                                                                                                  \
  extern inline size_t max_depth;                                                                 \
                                                                                                  \
  template <>                                                                                     \
  inline constexpr bool _is_exportable_object<type> = true;                                       \
                                                                                                  \
  template <typename T>                                                                           \
  std::string export_var(const T &, const std::string &, size_t, size_t, bool);                   \
                                                                                                  \
  template <typename T>                                                                           \
  inline auto export_object(const T &value, const std::string &indent, size_t last_line_length,   \
                            size_t current_depth, bool fail_on_newline)                           \
      -> std::enable_if_t<std::is_same_v<T, type>, std::string> {                                 \
    if (current_depth >= max_depth) return #type "{ ... }";                                       \
                                                                                                  \
    std::string new_indent = indent + "  ";                                                       \
    size_t next_depth = current_depth + 1;                                                        \
                                                                                                  \
    bool shift_indent = false;                                                                    \
                                                                                                  \
    std::string output;                                                                           \
    bool is_first;                                                                                \
                                                                                                  \
    auto append_output = [&](const std::string &property_name, const auto &property) -> void {    \
      if (is_first) {                                                                             \
        is_first = false;                                                                         \
      } else {                                                                                    \
        output += ", ";                                                                           \
      }                                                                                           \
                                                                                                  \
      if (shift_indent) {                                                                         \
        output += "\n" + new_indent + property_name + "= ";                                       \
        output += export_var(property, new_indent, _last_line_length(output), next_depth, false); \
      } else {                                                                                    \
        output += property_name + "= ";                                                           \
        output +=                                                                                 \
            export_var(property, indent, last_line_length + output.length(), next_depth, true);   \
      }                                                                                           \
    };                                                                                            \
                                                                                                  \
  rollback:                                                                                       \
    output = #type "{ ";                                                                          \
    is_first = true;                                                                              \
                                                                                                  \
    CPP_DUMP_EXPAND_VA_(CPP_DUMP_APPEND_OUTPUT_, __VA_ARGS__);                                    \
                                                                                                  \
    if (!shift_indent) {                                                                          \
      output += " }";                                                                             \
                                                                                                  \
      if (!_has_newline(output) && last_line_length + output.length() <= max_line_width)          \
        return output;                                                                            \
                                                                                                  \
      if (fail_on_newline) return "\n";                                                           \
                                                                                                  \
      shift_indent = true;                                                                        \
      goto rollback;                                                                              \
    }                                                                                             \
                                                                                                  \
    output += "\n" + indent + "}";                                                                \
                                                                                                  \
    return output;                                                                                \
  }                                                                                               \
                                                                                                  \
  }  // namespace cpp_dump

#define CPP_DUMP_REGISTER_EXPORT_OBJECT()                                                     \
  namespace cpp_dump {                                                                        \
                                                                                              \
  template <typename T>                                                                       \
  inline auto _export_var(const T &value, const std::string &indent, size_t last_line_length, \
                          size_t current_depth, bool fail_on_newline)                         \
      -> std::enable_if_t<is_exportable_object<T>, std::string> {                             \
    return export_object(value, indent, last_line_length, current_depth, fail_on_newline);    \
  }                                                                                           \
                                                                                              \
  }  // namespace cpp_dump
