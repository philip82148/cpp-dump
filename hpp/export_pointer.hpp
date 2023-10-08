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

#include "./escape_sequence.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename... Args>
inline std::string export_pointer(
    const std::weak_ptr<Args...> &pointer,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  return export_var(pointer.lock(), indent, last_line_length, current_depth, fail_on_newline);
}

template <typename T>
inline auto export_pointer(
    const T &pointer,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_pointer<T>, std::string> {
  if (pointer == nullptr) return es::reserved("nullptr");

  if constexpr (is_null_pointer<T> || !is_exportable<remove_pointer<T>>) {
    std::ostringstream ss;
    ss << std::hex << pointer;

    // Make the entire string an identifier
    return es::identifier(ss.str());
  } else {
    return es::identifier("*")
           + export_var(*pointer, indent, last_line_length + 1, current_depth, fail_on_newline);
  }
}

}  // namespace _detail

}  // namespace cpp_dump
