/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <optional>
#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

inline std::string
export_optional(const std::nullopt_t &, const std::string &, std::size_t, std::size_t, bool, const export_command &) {
  return es::identifier("std::nullopt");
}

template <typename T>
inline auto export_optional(
    const T &optional,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_optional<T>, std::string> {
  if (!optional) return es::identifier("std::nullopt");

  return es::identifier("?")
         + export_var(
             optional.value(), indent, last_line_length + 1, current_depth, fail_on_newline, command
         );
}

}  // namespace _detail

}  // namespace cpp_dump
