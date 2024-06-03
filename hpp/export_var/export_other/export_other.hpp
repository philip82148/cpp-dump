/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <complex>
#include <string>
#include <type_traits>
#include <variant>

#include "../../escape_sequence.hpp"
#include "../../export_command/export_command.hpp"
#include "../../type_check.hpp"
#include "../export_var_fwd.hpp"
#include "./export_es_value_t.hpp"
#include "./export_optional.hpp"
#include "./export_other_object.hpp"
#include "./export_type_info.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto export_other(
    const T &optional,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_optional<T>, std::string> {
  return export_optional(
      optional, indent, last_line_length, current_depth, fail_on_newline, command
  );
}

template <typename T>
inline auto export_other(
    const T &type_info,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_type_info<T>, std::string> {
  return export_type_info(
      type_info, indent, last_line_length, current_depth, fail_on_newline, command
  );
}

template <typename... Args>
inline std::string export_other(
    const std::reference_wrapper<Args...> &ref,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_var(ref.get(), indent, last_line_length, current_depth, fail_on_newline, command);
}

template <std::size_t N>
inline std::string
export_other(const std::bitset<N> &bitset, const std::string &, std::size_t, std::size_t, bool, const export_command &) {
  constexpr unsigned int chunk = 4;

  std::string bitset_str = bitset.to_string();
  std::string output;
  output.reserve(3 + N + (N - 1) / chunk);
  output.append("0b ");

  std::size_t pos = bitset_str.length() % chunk;
  if (pos > 0) output.append(bitset_str, 0, pos);
  for (; pos < bitset_str.length(); pos += chunk) {
    if (pos > 0) output.append(1, ' ');
    output.append(bitset_str, pos, chunk);
  }

  // Make the entire string an identifier
  return es::identifier(output);
}

template <typename T>
inline std::string
export_other(const std::complex<T> &complex, const std::string &, std::size_t, std::size_t current_depth, bool, const export_command &) {
  constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);

  auto imag = std::imag(complex);
  auto imag_sign = imag >= 0 ? "+" : "-";

  // Treat the entire complex string as an identifier
  return es::identifier(
             std::to_string(std::real(complex)) + " " + imag_sign + " "
             + std::to_string(std::abs(imag)) + "i "
         )
         + es::bracket("( ", current_depth) + es::member("abs") + es::op("= ")
         + es::number(std::to_string(std::abs(complex))) + es::op(", ") + es::member("arg/pi")
         + es::op("= ") + es::number(std::to_string(std::arg(complex) / pi))
         + es::bracket(" )", current_depth);
}

template <typename... Args>
inline std::string export_other(
    const std::variant<Args...> &variant,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return std::visit(
      [=, &indent, &command](const auto &value) -> std::string {
        return es::identifier("|")
               + export_var(
                   value, indent, last_line_length + 1, current_depth, fail_on_newline, command
               );
      },
      variant
  );
}

inline std::string export_other(
    const es_value_t &esv,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_es_value_t(esv, indent, last_line_length, current_depth, fail_on_newline, command);
}

template <typename T>
inline auto export_other(
    const T &value,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_other_object<T>, std::string> {
  return export_other_object(
      value, indent, last_line_length, current_depth, fail_on_newline, command
  );
}

}  // namespace _detail

}  // namespace cpp_dump
