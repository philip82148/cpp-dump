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

#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

inline std::string export_optional(const std::nullopt_t &, const std::string &, size_t, size_t,
                                   bool) {
  return "std::nullopt";
}

template <typename T>
inline auto export_optional(const T &optional, const std::string &indent, size_t last_line_length,
                            size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_optional<T>, std::string> {
  if (!optional) return "std::nullopt";

  return "?" +
         export_var(optional.value(), indent, last_line_length + 1, current_depth, fail_on_newline);
}

}  // namespace _detail

}  // namespace cpp_dump
