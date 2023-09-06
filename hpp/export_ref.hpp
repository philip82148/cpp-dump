/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./type_check.hpp"

namespace cpp_dump {

extern inline size_t max_depth;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
inline auto export_ref(const T &ref, const std::string &indent, size_t last_line_length,
                       size_t current_depth, bool fail_on_newline)
    -> std::enable_if_t<is_ref<T>, std::string> {
  return export_var(ref.get(), indent, last_line_length, current_depth, fail_on_newline);
}

}  // namespace _detail

}  // namespace cpp_dump
