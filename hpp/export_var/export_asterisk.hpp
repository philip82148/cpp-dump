/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>
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

inline std::string _asterisk_asterisk(std::string_view s) {
  return es_style == es_style_t::original ? es::identifier(s) : es::op(s);
}

}  // namespace es

template <typename T>
inline auto export_asterisk(
    const T &value,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_asterisk<T>, std::string> {
  if (!enable_asterisk) return export_unsupported();

  if (current_depth >= max_depth) return es::_asterisk_asterisk("*") + es::op("...");

  return es::_asterisk_asterisk("*")
         + export_var(
             *value, indent, last_line_length + 1, current_depth + 1, fail_on_newline, command
         );
}

}  // namespace _detail

}  // namespace cpp_dump
