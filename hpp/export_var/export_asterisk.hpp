/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./escape_sequence.hpp"
#include "./export_command.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

extern inline std::size_t max_depth;

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

template <typename T>
inline auto export_asterisk(
    const T &value,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_asterisk<T>, std::string> {
  if (current_depth >= max_depth) return "*...";

  // If decltype(*value) == decltype(value), then the program enters an infinite loop.
  // So increment depth.
  return es::identifier("*")
         + export_var(
             *value, indent, last_line_length + 1, current_depth + 1, fail_on_newline, command
         );
}

}  // namespace _detail

}  // namespace cpp_dump
