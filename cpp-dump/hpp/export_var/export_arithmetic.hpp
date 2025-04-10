/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <cctype>
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

namespace _export_arithmetic {

inline std::string export_arithmetic(
    bool bool_value,
    const std::string &,
    std::size_t,
    std::size_t,
    bool,
    const export_command &command
) {
  using bool_style_t = export_command::bool_style_t;
  switch (command.bool_style()) {
    case bool_style_t::normal:
      return es::reserved(bool_value ? "true" : "false");
    case bool_style_t::true_left:
      return es::reserved(bool_value ? "true " : "false");
    case bool_style_t::true_right:
      return es::reserved(bool_value ? " true" : "false");
    default:
      return es::number(bool_value ? "1" : "0");
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
    char char_value,
    const std::string &,
    std::size_t,
    std::size_t,
    bool,
    const export_command &command
) {
  const bool is_printable = std::isprint(static_cast<unsigned char>(char_value));
  const bool need_escape = !is_printable || char_value == '\'' || char_value == '\\';

  std::string output;
  // Escape if needed.
  if (need_escape) {
    auto escaped_char =
        is_printable ? std::string({'\\', char_value}) : escape_non_printable_char(char_value);
    if (command.char_as_hex() && escaped_char.size() > 2) {
      output = std::string(4, ' ');
    } else {
      output = es::character("'") + es::escaped_char(escaped_char) + es::character("'");
    }
  } else {
    char quoted_char[] = {'\'', char_value, '\''};
    output = es::character({quoted_char, sizeof(quoted_char)});
  }
  if (!command.char_as_hex()) {
    return output;
  }

  // The hex value is needed.

  // If the value is not escaped, add space before the hex value.
  if (!need_escape) {
    output.push_back(' ');
  }

  // Add the hex value.
  auto to_hex_char = [](unsigned char c) -> char {
    return static_cast<char>(c < 10 ? '0' + c : 'A' + (c - 10));
  };
  char upper = to_hex_char((char_value >> 4) & 0x0f);
  char lower = to_hex_char(char_value & 0x0f);
  char number[] = {'0', 'x', upper, lower};
  output.append(es::number({number, sizeof(number)}));

  return output;
}

// helper for _get_max_digits
template <typename UnsignedT>
constexpr unsigned int _get_max_digits_aux(UnsignedT num, unsigned int base) {
  return num == 0 ? 0 : 1 + _get_max_digits_aux(num / base, base);
}

// helper for export_arithmetic(T)
// Get the maximum digits of the T with base as the radix.
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

// helper for export_arithmetic(T)
// Convert the absolute integer to a reversed string.
template <typename T, typename UnsignedTOrInt>
std::string _abs_to_reversed_str(UnsignedTOrInt abs, unsigned int base) {
  std::string reversed;
  if (base == 10) {
    reversed = std::to_string(abs);
    std::reverse(reversed.begin(), reversed.end());
  } else if (base == 2) {
    // +3 is for the prefix.
    reversed.reserve(sizeof(T) * 8 + 3);
    bool is_first = true;
    while (is_first || abs) {
      is_first = false;
      char next_digit = static_cast<char>((abs & 0x01) + '0');
      reversed.push_back(next_digit);
      abs >>= 1;
    }
  } else {
    std::stringstream ss;
    ss << std::setbase(base) << std::uppercase << abs;
    reversed = ss.str();
    std::reverse(reversed.begin(), reversed.end());
  }
  return reversed;
}

// helper for export_arithmetic(T)
// Add spaces between chunks.
inline std::string _chunk_string(std::string_view input, int base, int chunk) {
  std::string output;
  output.reserve(input.size() * 2);
  for (std::size_t pos = 0; pos < input.size(); pos += chunk) {
    output.append(input, pos, chunk);
    output.push_back(' ');
  }
  if (base == 10) {
    output.pop_back();
  }
  return output;
}

// helper for export_arithmetic(T)
inline const char *_get_reversed_prefix(int base) {
  // base == 10 is handled before this function.
  if (base == 2) {
    return "b0";
  } else if (base == 8) {
    return "o0";
  } else {
    return "x0";
  }
}

template <typename T>
inline auto export_arithmetic(
    T value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) -> std::enable_if_t<std::is_integral_v<T>, std::string> {
  auto int_style_ = command.int_style();
  // If int_style is not specified, export with no style.
  if (!int_style_) {
    std::string output = command.format(value);
    if (output.empty()) {
      return es::signed_number(std::to_string(value));
    }
    return es::signed_number(output);
  }

  auto [base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus] = int_style_.value();
  // If base is 10 and the other values are not specified, export with no style.
  if (base == 10 && digits == 0 && chunk == 0) {
    return es::signed_number(std::to_string(value));
  }

  // Style the integer with int_style.

  // Declare variables.
  unsigned int max_digits = _get_max_digits<T>(base);
  if (digits > max_digits) {
    digits = max_digits;
  }
  if (chunk > max_digits) {
    chunk = 0;
  }
  const bool make_unsigned =
      std::is_signed_v<T> && base != 10 && make_unsigned_or_no_space_for_minus;
  const bool add_extra_space = !(std::is_unsigned_v<T> || make_unsigned_or_no_space_for_minus);
  using UnsignedT = std::make_unsigned_t<T>;
  // Let stringstream recognize the type as an integer.
  using UnsignedTOrInt =
      std::conditional_t<(sizeof(UnsignedT) > sizeof(unsigned int)), UnsignedT, unsigned int>;
  UnsignedTOrInt abs;
  if constexpr (std::is_signed_v<T>) {
    abs = static_cast<UnsignedTOrInt>(
        make_unsigned ? static_cast<UnsignedT>(value) : std::abs(value)
    );
  } else {
    abs = value;
  }

  // Create a (reversed) string of the abs with base as the radix
  std::string output = _abs_to_reversed_str<T, UnsignedTOrInt>(abs, base);

  // Add a minus before filling when needed
  const bool need_minus = !make_unsigned && value < 0;
  const bool minus_before_fill = base == 10 && space_fill;
  if (need_minus && minus_before_fill) {
    output.push_back('-');
  }

  // Fill with spaces/zeros to make the length `digits`
  if (output.size() < digits) {
    output.append(digits - output.size(), space_fill ? ' ' : '0');
  }

  const bool length_was_below_digits = output.size() <= digits;
  // Add a space between chunks
  if (chunk > 0) {
    output = _chunk_string(output, base, chunk);
  }

  // Add prefix.
  if (base != 10) {
    output.append(_get_reversed_prefix(base));
  }

  // Add a minus after filling when needed
  if (need_minus && !minus_before_fill) {
    output.push_back('-');
  } else if (length_was_below_digits && add_extra_space) {
    output.push_back(' ');
  }

  // Reverse the output and add color and suffix.
  std::reverse(output.begin(), output.end());
  output = es::signed_number(output);
  if (make_unsigned) {
    output.append(es::op(" u"));
  }

  return output;
}

template <typename T>
inline auto export_arithmetic(
    T value, const std::string &, std::size_t, std::size_t, bool, const export_command &command
) -> std::enable_if_t<std::is_floating_point_v<T>, std::string> {
  std::string output = command.format(value);
  if (output.empty()) {
    return es::signed_number(std::to_string(value));
  }
  return es::signed_number(output);
}

}  // namespace _export_arithmetic

using _export_arithmetic::export_arithmetic;

}  // namespace _detail

}  // namespace cpp_dump
