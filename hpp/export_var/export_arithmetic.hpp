/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <limits>
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

template <typename UnsignedT>
constexpr unsigned int _get_max_digits_aux(UnsignedT num, unsigned int base) {
  return num == 0 ? 0 : 1 + _get_max_digits_aux(num / base, base);
}

template <typename T>
unsigned int _get_max_digits(unsigned int base) {
  using UnsignedT = std::make_unsigned_t<T>;
  constexpr UnsignedT T_max =
      std::max<UnsignedT>(std::numeric_limits<T>::max(), std::numeric_limits<T>::min());

  switch (base) {
    case 2: {
      constexpr unsigned int max_digits = _get_max_digits_aux(T_max, 2);
      return max_digits;
    }
    case 8: {
      constexpr unsigned int max_digits = _get_max_digits_aux(T_max, 8);
      return max_digits;
    }
    case 10: {
      constexpr unsigned int max_digits = _get_max_digits_aux(T_max, 10);
      return max_digits;
    }
    default: {
      constexpr unsigned int max_digits = _get_max_digits_aux(T_max, 16);
      return max_digits;
    }
  }
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

  // make_unsigned is for future implementation.
  auto [base, digits, chunk, space_fill, make_unsigned] = int_style.value();

  if (base == 10 && digits == 0 && chunk == 0) return es::number(std::to_string(value));

  unsigned int max_digits = _get_max_digits<T>(base);
  if (digits > max_digits) digits = max_digits;
  if (chunk > max_digits) chunk = 0;

  std::string output;

  using UnsignedT = std::make_unsigned_t<T>;
  using UnsignedTmpType =
      std::conditional_t<std::is_same_v<UnsignedT, unsigned char>, unsigned int, UnsignedT>;

  UnsignedTmpType unsigned_tmp;
  if constexpr (std::is_unsigned_v<T>) {
    unsigned_tmp = value;
  } else {
    unsigned_tmp = static_cast<UnsignedTmpType>(make_unsigned ? value : std::abs(value));
  }

  bool space_for_minus = !(std::is_unsigned_v<T> || make_unsigned);

  // Create a string of an integer with base as the radix
  if (base == 10) {
    output = std::to_string(unsigned_tmp);
    std::reverse(output.begin(), output.end());
  } else if (base == 2) {
    output.reserve(digits > 0 ? digits + (chunk == 0 && space_for_minus) : sizeof(T) * 8 + 1);

    bool is_first = true;
    while (is_first || unsigned_tmp) {
      is_first = false;
      char next_digit = static_cast<char>((unsigned_tmp & 0x01) + '0');
      output.append(1, next_digit);
      unsigned_tmp >>= 1;
    }
  } else {
    std::stringstream ss;
    ss << std::setbase(base) << unsigned_tmp;
    output = ss.str();
    std::reverse(output.begin(), output.end());
  }

  // Add a minus when value < 0 (part 1)
  bool need_minus = !make_unsigned && value < 0;
  bool added_minus_before_fill = space_fill && (digits == 0 || output.length() < digits);
  if (need_minus && added_minus_before_fill) output.append(1, '-');

  if (output.length() < digits) {
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
    std::string new_output;
    new_output.reserve(output.size() + (output.size() - 1) / chunk + space_for_minus);

    for (std::size_t pos = 0; pos < output.size(); pos += chunk) {
      if (pos > 0) new_output.append(1, ' ');
      new_output.append(output, pos, chunk);
    }

    output.swap(new_output);
  }

  // Add a minus when value < 0 (part 2)
  if (need_minus && !added_minus_before_fill) {
    output.append(1, '-');
  } else if (space_for_minus && length_was_below_digits) {
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
