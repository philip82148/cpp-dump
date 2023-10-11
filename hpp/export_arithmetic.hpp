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

namespace _detail {

inline std::string
export_arithmetic(bool value, const std::string &, size_t, size_t, bool, const export_command &) {
  return es::reserved(value ? "true" : "false");
}

inline std::string
export_arithmetic(char value, const std::string &, size_t, size_t, bool, const export_command &) {
  return es::character("'" + std::string{value} + "'");
}

template <typename T>
inline auto
export_arithmetic(const T &value, const std::string &, size_t, size_t, bool, const export_command &)
    -> std::enable_if_t<is_arithmetic<T>, std::string> {
  return es::number(std::to_string(value));
}

}  // namespace _detail

}  // namespace cpp_dump
