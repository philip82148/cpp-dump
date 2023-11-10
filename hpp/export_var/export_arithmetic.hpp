/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <string>
#include <tuple>
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
  if (!int_style) return es::number(std::to_string(value));

  auto [base, digits, chunk, space_fill, support_negative] = int_style.value();

  std::string output;

  T tmp;
  if constexpr (std::is_signed_v<T>) {
    tmp = std::abs(value);
  } else {
    tmp = value;
  }

  if (base == 10) {
    output = std::to_string(tmp);
    std::reverse(output.begin(), output.end());
  } else {
    bool is_first = true;
    while (is_first || tmp) {
      is_first = false;
      T r;
      switch (base) {
        case 2:
          r = tmp & 0x01;
          break;
        case 4:
          r = tmp & 0x03;
          break;
        case 8:
          r = tmp & 0x07;
          break;
        case 16:
          r = tmp & 0x0f;
          break;
        default:
          r = tmp % static_cast<T>(base);
          break;
      }

      if (r <= 9) {
        output.append(1, static_cast<char>(r + '0'));
      } else {
        output.append(1, static_cast<char>(r - 10 + 'a'));
      }

      switch (base) {
        case 2:
          tmp >>= 1;
          break;
        case 4:
          tmp >>= 2;
          break;
        case 8:
          tmp >>= 3;
          break;
        case 16:
          tmp >>= 4;
          break;
        default:
          tmp /= static_cast<T>(base);
          break;
      }
    }
  }

  bool add_minus = false;
  if (space_fill && value < 0 && (digits == 0 || output.length() < digits)) {
    output.append(1, '-');
    add_minus = true;
  }

  if (digits > 0 && output.length() < digits) {
    if (space_fill) {
      output.append(digits - output.length(), ' ');
    } else {
      output.append(digits - output.length(), '0');
    }
  }

  bool less_digits_plus1 = output.length() < digits + 1;
  if (chunk > 0) {
    std::string output_tmp;
    output.swap(output_tmp);
    for (std::size_t begin = 0; begin < output_tmp.length(); begin += chunk) {
      std::size_t length = std::min<std::size_t>(chunk, output_tmp.length() - begin);
      if (begin != 0) output.append(1, ' ');
      output.append(output_tmp.substr(begin, length));
    }
  }

  if (!add_minus && value < 0) {
    output.append(1, '-');
  } else if (digits > 0 && support_negative && less_digits_plus1) {
    output.append(1, ' ');
  }

  std::reverse(output.begin(), output.end());

  if (base == 10) return es::number(output);

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
