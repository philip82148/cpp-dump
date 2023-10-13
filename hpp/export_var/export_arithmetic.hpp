/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"

namespace cpp_dump {

namespace _detail {

inline std::string
export_arithmetic(bool value, const std::string &, std::size_t, std::size_t, bool, const export_command &) {
  return es::reserved(value ? "true" : "false");
}

inline std::string
export_arithmetic(char value, const std::string &, std::size_t, std::size_t, bool, const export_command &) {
  return es::character("'" + std::string{value} + "'");
}

template <typename T>
inline auto export_arithmetic(
    const T &value,
    const std::string &,
    std::size_t,
    std::size_t,
    bool,
    const export_command &command
) -> std::enable_if_t<is_arithmetic<T> && std::is_integral_v<T>, std::string> {
  auto int_style = command.get_int_style();
  T base = static_cast<T>(int_style.first), chunk = static_cast<T>(int_style.second);
  if (base == 0 || value < 0) return es::number(std::to_string(value));

  std::string output;
  T tmp = value;
  int i = 0;
  for (; i == 0 || tmp; ++i) {
    T r = tmp % base;
    if (chunk > 0 && i > 0 && i % chunk == 0) output = " " + output;
    if (r <= 9) {
      output = static_cast<char>(r + '0') + output;
    } else {
      output = static_cast<char>(r - 10 + 'a') + output;
    }
    tmp /= base;
  }
  for (; chunk > 0 && i % chunk; ++i) output = "0" + output;

  return es::number(output) + es::op(" _" + std::to_string(base));
}

template <typename T>
inline auto
export_arithmetic(const T &value, const std::string &, std::size_t, std::size_t, bool, const export_command &)
    -> std::enable_if_t<is_arithmetic<T> && !std::is_integral_v<T>, std::string> {
  return es::number(std::to_string(value));
}

}  // namespace _detail

}  // namespace cpp_dump
