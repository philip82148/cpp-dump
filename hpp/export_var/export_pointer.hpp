/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../options.hpp"
#include "../type_check.hpp"
#include "./export_unsupported.hpp"
#include "./export_var_fwd.hpp"

namespace cpp_dump {

namespace _detail {

namespace es {

inline std::string _ptr_asterisk(const std::string &s) {
  return es_style == es_style_t::by_syntax ? es::identifier(s) : es::op(s);
}

inline std::string _raw_address(const std::string &s) {
  return es_style == es_style_t::by_syntax ? es::identifier(s) : es::number(s);
}

}  // namespace es

template <typename... Args>
inline std::string export_pointer(
    const std::weak_ptr<Args...> &pointer,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_var(
      pointer.lock(), indent, last_line_length, current_depth, fail_on_newline, command
  );
}

template <typename T>
inline auto export_pointer(
    const T &pointer,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_pointer<T>, std::string> {
  if (pointer == nullptr) return es::reserved("nullptr");

  if constexpr (is_null_pointer<T> || !is_exportable<remove_pointer<T>>) {
    if constexpr (std::is_function_v<remove_pointer<T>>) {
      return export_unsupported();
    } else {
      std::ostringstream ss;
      ss << std::hex << pointer;

      // Make the entire string an identifier
      return es::_raw_address(ss.str());
    }
  } else {
    return es::_ptr_asterisk("*")
           + export_var(
               *pointer, indent, last_line_length + 1, current_depth, fail_on_newline, command
           );
  }
}

}  // namespace _detail

}  // namespace cpp_dump
