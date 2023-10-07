/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <stdexcept>

#include "./escape_sequence.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

#define _p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(TYPE)                                                  \
  inline std::string export_exception(                                                             \
      const TYPE &exception,                                                                       \
      const std::string &indent,                                                                   \
      size_t last_line_length,                                                                     \
      size_t current_depth,                                                                        \
      bool fail_on_newline                                                                         \
  ) {                                                                                              \
    size_t next_depth = current_depth + 1;                                                         \
                                                                                                   \
    std::string prefix = with_es::identifier(#TYPE) + with_es::bracket("{ ", current_depth)        \
                         + with_es::member("what()") + "= ";                                       \
    std::string output =                                                                           \
        prefix                                                                                     \
        + export_var(                                                                              \
            exception.what(), indent, last_line_length + get_length(prefix), next_depth, true      \
        )                                                                                          \
        + with_es::bracket(" }", current_depth);                                                   \
                                                                                                   \
    if (!has_newline(output) && get_length(output) <= max_line_width) return output;               \
                                                                                                   \
    if (fail_on_newline) return "\n";                                                              \
                                                                                                   \
    std::string new_indent = indent + "  ";                                                        \
                                                                                                   \
    prefix = new_indent + with_es::member("what()") + "= ";                                        \
    output = with_es::identifier(#TYPE) + with_es::bracket("{\n", current_depth) + prefix          \
             + export_var(exception.what(), new_indent, get_length(prefix), next_depth, false)     \
             + "\n" + indent + with_es::bracket("}", current_depth);                               \
                                                                                                   \
    return output;                                                                                 \
  }

namespace cpp_dump {

extern inline size_t max_line_width;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::logic_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::domain_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::invalid_argument);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::length_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::out_of_range);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::runtime_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::range_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::overflow_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::underflow_error);
_p_CPP_DUMP_DEFINE_EXPORT_EXCEPTION(std::exception);

template <typename T>
inline auto export_exception(
    const T &exception,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_exception<T>, std::string> {
  size_t next_depth = current_depth + 1;

  std::string prefix = with_es::bracket("{ ", current_depth) + with_es::member("what()") + "= ";
  std::string output =
      prefix
      + export_var(
          exception.what(), indent, last_line_length + get_length(prefix), next_depth, true
      )
      + with_es::bracket(" }", current_depth);

  if (!has_newline(output) && get_length(output) <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + with_es::member("what()") + "= ";
  output = with_es::bracket("{\n", current_depth) + prefix
           + export_var(exception.what(), new_indent, get_length(prefix), next_depth, false) + "\n"
           + indent + with_es::bracket("}", current_depth);
}

}  // namespace _detail

}  // namespace cpp_dump
