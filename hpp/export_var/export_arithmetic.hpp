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
  auto [base, digits, chunk, space_fill, support_negative] = command.get_int_style();
  if (base == 0 || (!support_negative && value < 0)) return es::number(std::to_string(value));

  std::string output;
  T tmp;
  if constexpr (std::is_signed_v<T>) {
    tmp = std::abs(value);
  } else {
    tmp = value;
  }

  if (base == 10) {
    std::string output_tmp = std::to_string(tmp);

    if (digits > 0 && digits > output_tmp.length()) {
      if (space_fill) {
        output_tmp.insert(0, digits - output_tmp.length(), ' ');
      } else {
        output_tmp.insert(0, digits - output_tmp.length(), '0');
      }
    }

    if (support_negative) {
      if (value >= 0)
        output = ' ';
      else
        output = '-';
    }

    if (chunk > 0) {
      std::size_t begin = output_tmp.length() % chunk;
      if (begin != 0) output.append(output_tmp.substr(0, begin));
      for (; begin < output_tmp.length(); begin += chunk) {
        std::size_t length = std::min<std::size_t>(chunk, output_tmp.length() - begin);
        if (begin != 0) output.append(1, ' ');
        output.append(output_tmp.substr(begin, length));
      }
    } else {
      output.append(output_tmp);
    }

    return es::number(output);
  }

  unsigned int digit_i = 0, next_digit_of_chunk = chunk;
  for (; digit_i == 0 || tmp; ++digit_i) {
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

    if (chunk > 0 && digit_i >= next_digit_of_chunk) {
      output.append(1, ' ');
      next_digit_of_chunk += chunk;
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

  if (digits > 0 && digit_i < digits) {
    if (!space_fill && chunk > 0) {
      unsigned int fill_length = chunk - digit_i % chunk;
      if (fill_length != chunk) {
        fill_length = std::min(chunk - digit_i % chunk, digits - digit_i);
        output.append(fill_length, '0');
        digit_i += fill_length;
      }

      for (; digit_i < digits; digit_i += chunk) {
        int length = std::min(chunk, digits - digit_i);
        output.append(1, ' ');
        output.append(length, '0');
      }
    } else {
      int fill_length = digits - digit_i;
      if (space_fill) {
        if (chunk > 0) fill_length += ((digits - 1) / chunk) - ((digit_i - 1) / chunk);
        output.append(fill_length, ' ');
      } else {
        output.append(fill_length, '0');
      }
    }
  }

  if (support_negative) {
    if (value >= 0)
      output.append(1, ' ');
    else
      output.append(1, '-');
  }

  std::reverse(output.begin(), output.end());

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
