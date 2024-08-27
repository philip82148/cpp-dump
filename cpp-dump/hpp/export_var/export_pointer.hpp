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

inline std::string _ptr_asterisk(std::string_view s) {
  return options::es_style == types::es_style_t::original ? es::identifier(s) : es::op(s);
}

inline std::string _raw_address(std::string_view s) {
  return options::es_style == types::es_style_t::original ? es::identifier(s) : es::number(s);
}

}  // namespace es

template <typename T>
inline auto export_pointer(
    const T &pointer,
    [[maybe_unused]] const std::string &indent,
    [[maybe_unused]] std::size_t last_line_length,
    [[maybe_unused]] std::size_t current_depth,
    [[maybe_unused]] bool fail_on_newline,
    [[maybe_unused]] const export_command &command
) -> std::enable_if_t<is_pointer<T>, std::string> {
  if (pointer == nullptr) return es::reserved("nullptr");

  if constexpr (is_null_pointer<T> || !is_exportable<remove_pointer<T>>) {
    if constexpr (std::is_function_v<remove_pointer<T>>) {
      return export_unsupported();
    } else {
      std::ostringstream ss;
      ss << std::hex << static_cast<const void *>(pointer);

      // Make the entire string an identifier
      return es::_raw_address(ss.str());
    }
  } else {
    if (current_depth >= command.addr_depth()) {
      std::ostringstream ss;

      if constexpr (is_smart_pointer<T>) {
        ss << std::hex << static_cast<const void *>(pointer.get());
      } else {
        ss << std::hex << static_cast<const void *>(pointer);
      }

      // Make the entire string an identifier
      return es::_raw_address(ss.str());
    }

    if (current_depth >= options::max_depth) return es::_ptr_asterisk("*") + es::op("...");

    return es::_ptr_asterisk("*")
           + export_var(
               *pointer, indent, last_line_length + 1, current_depth + 1, fail_on_newline, command
           );
  }
}

template <typename... Args>
inline std::string export_pointer(
    const std::weak_ptr<Args...> &pointer,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_pointer(
      pointer.lock(), indent, last_line_length, current_depth, fail_on_newline, command
  );
}

}  // namespace _detail

}  // namespace cpp_dump
