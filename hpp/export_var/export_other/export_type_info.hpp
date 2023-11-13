/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

#include "../../export_command/export_command.hpp"
#include "../../type_check.hpp"
#include "../export_object_common.hpp"

_p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto export_type_info(
    const T &type_info,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_type_info<T>, std::string> {
  std::string type_name = get_typename<T>();

#if defined(__GNUC__)
  int status = 0;
  std::string name = abi::__cxa_demangle(type_info.name(), 0, 0, &status);
#else
  std::string name = type_info.name();
#endif

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;

  append_output("name()", name);

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON3;
}

}  // namespace _detail

}  // namespace cpp_dump
