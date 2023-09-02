/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <sstream>
#include <string>
#include <type_traits>

#include "./type_check.hpp"

namespace cpp_dump {

extern inline size_t max_depth;

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
auto export_pointer(const T &value, const std::string &indent, size_t last_line_length,
                    size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_pointer<T>, std::string> {
  if (value == nullptr) return "nullptr";

  if constexpr (is_void_pointer<T>) {
    std::ostringstream ss;
    ss << std::hex << value;

    return ss.str();
  } else {
    if (current_depth >= max_depth) return "*...";

    return "*" +
           export_var(*value, indent, last_line_length + 1, current_depth + 1, fail_on_newline);
  }
}

}  // namespace cpp_dump
