/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <string>
#include <type_traits>
#include <variant>

#include "./escape_sequence.hpp"
#include "./export_command.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, size_t, size_t, bool, const export_command &);

template <typename... Args>
inline std::string export_other(
    const std::reference_wrapper<Args...> &ref,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command

) {
  return export_var(ref.get(), indent, last_line_length, current_depth, fail_on_newline, command);
}

template <size_t N>
inline std::string export_other(
    const std::bitset<N> &bitset, const std::string &, size_t, size_t, bool, const export_command &

) {
  std::string bitset_str = bitset.to_string();

  std::string output;
  for (int end = bitset_str.length(); end > 0; end -= 4) {
    size_t begin = std::max(end - 4, 0);
    if (output == "") {
      output = bitset_str.substr(begin, end - begin);
    } else {
      output = bitset_str.substr(begin, end - begin) + " " + output;
    }
  }
  output = "0b " + output;

  // Make the entire string an identifier
  return es::identifier(output);
}

template <typename... Args>
inline std::string export_other(
    const std::complex<Args...> &complex,
    const std::string &,
    size_t,
    size_t current_depth,
    bool,
    const export_command &

) {
  auto imag      = std::imag(complex);
  auto imag_sign = imag >= 0 ? "+" : "-";

  // Treat the entire complex string as an identifier
  return es::identifier(
             std::to_string(std::real(complex)) + " " + imag_sign + " "
             + std::to_string(std::abs(imag)) + "i "
         )
         + es::bracket("( ", current_depth) + es::member("abs") + es::op("= ")
         + es::number(std::to_string(std::abs(complex))) + es::op(", ") + es::member("arg/pi")
         + es::op("= ") + es::number(std::to_string(std::arg(complex) / M_PI))
         + es::bracket(" )", current_depth);
}

template <typename... Args>
inline std::string export_other(
    const std::variant<Args...> &variant,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command

) {
  return std::visit(
      [=, &indent](const auto &value) -> std::string {
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
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline,
    const export_command &command

) {
  return export_es_value_t(esv, indent, last_line_length, current_depth, fail_on_newline);
}

}  // namespace _detail

}  // namespace cpp_dump
