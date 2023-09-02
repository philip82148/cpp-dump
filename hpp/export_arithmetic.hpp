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

inline std::string export_arithmetic(bool value, const std::string &, size_t, size_t, bool) {
  return value ? "true" : "false";
}

inline std::string export_arithmetic(char value, const std::string &, size_t, size_t, bool) {
  return "'" + std::string{value} + "'";
}

template <typename T>
inline auto export_arithmetic(const T &value, const std::string &, size_t, size_t, bool)
    -> std::enable_if_t<is_arithmetic<T>, std::string> {
  return std::to_string(value);
}

}  // namespace cpp_dump
