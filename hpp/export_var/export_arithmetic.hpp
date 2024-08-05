/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <iomanip>
#include <ios>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"

namespace cpp_dump {

namespace _detail {

inline std::string export_arithmetic(
    bool value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) {
  using bool_style_t = export_command::bool_style_t;
  switch (command.bool_style()) {
    case bool_style_t::normal:
      return es::reserved(value ? "true" : "false");
    case bool_style_t::true_left:
      return es::reserved(value ? "true " : "false");
    case bool_style_t::true_right:
      return es::reserved(value ? " true" : "false");
    default:
      return es::number(value ? "1" : "0");
  }
}

template <typename T>
inline auto export_arithmetic(
    const T &value,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_vector_bool_reference<T>, std::string> {
  return export_arithmetic(
      static_cast<bool>(value), indent, last_line_length, current_depth, fail_on_newline, command
  );
}

inline std::string export_arithmetic(
    char value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) {
  char quoted_char[] = {'\'', value, '\''};

  if (!command.char_as_hex()) return es::character({quoted_char, sizeof(quoted_char)});

  auto to_hex_char = [](unsigned char c) -> char {
    return static_cast<char>(c < 10 ? '0' + c : 'A' + (c - 10));
  };

  char upper = to_hex_char((value >> 4) & 0x0f);
  char lower = to_hex_char(value & 0x0f);

  char number[] = {'0', 'x', upper, lower};
  return es::character({quoted_char, sizeof(quoted_char)}) + es::op(" (")
         + es::number({number, sizeof(number)}) + es::op(")");
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
    T value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) -> std::enable_if_t<std::is_integral_v<T>, std::string> {
  std::string output = command.format(value);
  if (!output.empty()) return es::number(output);

  auto int_style_ = command.int_style();
  if (!int_style_) return es::number(std::to_string(value));

  auto [base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus] = int_style_.value();

  if (base == 10 && digits == 0 && chunk == 0) return es::number(std::to_string(value));

  unsigned int max_digits = _get_max_digits<T>(base);
  if (digits > max_digits) digits = max_digits;
  if (chunk > max_digits) chunk = 0;

  const bool make_unsigned =
      std::is_signed_v<T> && base != 10 && make_unsigned_or_no_space_for_minus;
  const bool add_extra_space = !(std::is_unsigned_v<T> || make_unsigned_or_no_space_for_minus);

  using UnsignedT = std::make_unsigned_t<T>;
  // Let stringstream recognize the type as an integer.
  using UnsignedTOrInt =
      std::conditional_t<std::is_same_v<UnsignedT, unsigned char>, unsigned int, UnsignedT>;

  UnsignedTOrInt unsigned_tmp;
  if constexpr (std::is_signed_v<T>) {
    unsigned_tmp = static_cast<UnsignedTOrInt>(
        make_unsigned ? static_cast<UnsignedT>(value) : std::abs(value)
    );
  } else {
    unsigned_tmp = value;
  }

  std::string_view reversed_prefix;

  // Create a string of an integer with base as the radix
  if (base == 10) {
    output = std::to_string(unsigned_tmp);
    std::reverse(output.begin(), output.end());
  } else if (base == 2) {
    output.reserve(sizeof(T) * 8 + 3);

    bool is_first = true;
    while (is_first || unsigned_tmp) {
      is_first = false;
      char next_digit = static_cast<char>((unsigned_tmp & 0x01) + '0');
      output.push_back(next_digit);
      unsigned_tmp >>= 1;
    }

    reversed_prefix = "b0";
  } else {
    std::stringstream ss;
    ss << std::setbase(base) << std::uppercase << unsigned_tmp;
    output = ss.str();
    std::reverse(output.begin(), output.end());

    reversed_prefix = base == 16 ? "x0" : "o0";
  }

  // Add a minus when value < 0 (part 1)
  const bool need_minus = !make_unsigned && value < 0;
  const bool minus_before_fill = base == 10 && space_fill;
  if (need_minus && minus_before_fill) output.push_back('-');

  if (output.size() < digits) {
    // Fill with spaces/zeros
    if (space_fill) {
      output.append(digits - output.size(), ' ');
    } else {
      output.append(digits - output.size(), '0');
    }
  }

  const bool length_was_below_digits = output.size() <= digits;
  if (chunk > 0) {
    // Add a space between chunks
    std::string new_output;
    new_output.reserve(output.size() * 2);

    for (std::size_t pos = 0; pos < output.size(); pos += chunk) {
      new_output.append(output, pos, chunk);
      new_output.push_back(' ');
    }
    if (base == 10) new_output.pop_back();

    output.swap(new_output);
  }

  output.append(reversed_prefix);

  // Add a minus when value < 0 (part 2)
  if (need_minus && !minus_before_fill) {
    output.push_back('-');
  } else if (length_was_below_digits && add_extra_space) {
    output.push_back(' ');
  }

  std::reverse(output.begin(), output.end());

  if (make_unsigned) return es::number(output) + es::op(" u");

  return es::number(output);
}

template <typename T>
inline auto export_arithmetic(
    T value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) -> std::enable_if_t<std::is_floating_point_v<T>, std::string> {
  std::string output = command.format(value);
  if (!output.empty()) return es::number(output);

  return es::number(std::to_string(value));
}

}  // namespace _detail

}  // namespace cpp_dump
