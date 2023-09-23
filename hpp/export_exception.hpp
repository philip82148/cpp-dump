/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <stdexcept>

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
    std::string prefix = #TYPE "{ what()= ";                                                       \
    std::string output =                                                                           \
        prefix                                                                                     \
        + export_var(                                                                              \
            exception.what(), indent, last_line_length + prefix.length(), next_depth, true         \
        )                                                                                          \
        + " }";                                                                                    \
                                                                                                   \
    if (!has_newline(output) && output.length() <= max_line_width) return output;                  \
                                                                                                   \
    if (fail_on_newline) return "\n";                                                              \
                                                                                                   \
    std::string new_indent = indent + "  ";                                                        \
                                                                                                   \
    prefix = new_indent + "what()= ";                                                              \
    output = #TYPE "{\n" + prefix                                                                  \
             + export_var(exception.what(), new_indent, prefix.length(), next_depth, false) + "\n" \
             + indent + "}";                                                                       \
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

  std::string prefix = "{ what()= ";
  std::string output =
      prefix
      + export_var(exception.what(), indent, last_line_length + prefix.length(), next_depth, true)
      + " }";

  if (!has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "what()= ";
  output = "{\n" + prefix
           + export_var(exception.what(), new_indent, prefix.length(), next_depth, false) + "\n"
           + indent + "}";

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
