/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <cstring>
#include <sstream>
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
  return es::character("'" + std::string({value}) + "'");
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

  if (base == 10 && digits == 0 && chunk == 0) return es::number(std::to_string(value));

  std::string output;

  std::make_unsigned_t<T> non_negative_tmp;
  if constexpr (std::is_signed_v<T>) {
    non_negative_tmp = static_cast<std::make_unsigned_t<T>>(std::abs(value));
  } else {
    non_negative_tmp = value;
  }

  // Create a string of an integer with base as the radix
  if (base == 10) {
    output = std::to_string(non_negative_tmp);
    std::reverse(output.begin(), output.end());
  } else if (base == 2) {
    bool is_first = true;
    while (is_first || non_negative_tmp) {
      is_first = false;
      char next_digit = (non_negative_tmp & 0x01) + '0';
      output.append(1, next_digit);
      non_negative_tmp >>= 1;
    }
  } else {
    std::stringstream ss;
    ss << std::setbase(base) << non_negative_tmp;
    output = ss.str();
    std::reverse(output.begin(), output.end());
  }

  // Add a minus when value < 0 (part 1)
  bool minus_before_fill = space_fill && (digits == 0 || output.length() < digits);
  if (minus_before_fill && value < 0) output.append(1, '-');

  if (digits > 0 && output.length() < digits) {
    // Fill with spaces/zeros
    if (space_fill) {
      output.append(digits - output.length(), ' ');
    } else {
      output.append(digits - output.length(), '0');
    }
  }

  bool length_was_below_digits = output.length() <= digits;
  if (chunk > 0) {
    // Add a space between chunks
    std::string new_output(output.size() + (output.size() - 1) / chunk, ' ');

    std::size_t new_pos = 0;
    for (std::size_t begin = 0; begin < output.length(); begin += chunk) {
      std::memcpy(new_output.data() + new_pos, output.substr(begin, chunk).c_str(), chunk);
      new_pos += chunk + 1;
    }

    output.swap(new_output);
  }

  // Add a minus when value < 0 (part 2) or a space when not and support_negative
  if (!minus_before_fill && value < 0) {
    output.append(1, '-');
  } else if (support_negative && length_was_below_digits) {
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
