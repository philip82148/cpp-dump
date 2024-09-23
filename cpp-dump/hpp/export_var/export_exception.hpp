/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "../type_name.hpp"
#include "./export_object_common.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto export_exception(
    const T &exception,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_exception<T>, std::string> {
  std::string class_name = es::class_name(styled_typename<T>());

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;

  append_output("what()", exception.what());

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}

}  // namespace _detail

}  // namespace cpp_dump
