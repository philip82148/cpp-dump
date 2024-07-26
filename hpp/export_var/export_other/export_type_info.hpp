/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>
#include <typeinfo>

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

#include "../../export_command/export_command.hpp"
#include "../export_object_common.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto
export_type_info(const T &type_info, const std::string &indent, std::size_t last_line_length, std::size_t current_depth, bool fail_on_newline, const export_command &)
    -> std::enable_if_t<is_type_info<T>, std::string> {
  std::string class_name =
      es::class_name(std::is_same_v<T, std::type_info> ? "std::type_info" : "std::type_index");

#if defined(__GNUC__)
  int status = 0;
  std::string name = abi::__cxa_demangle(type_info.name(), 0, 0, &status);
#else
  std::string name = type_info.name();
#endif

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1_1;
  [[maybe_unused]] auto _dummy_1 = next_depth;
  [[maybe_unused]] auto _dummy_2 = is_first = false;

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1_3;

  if (shift_indent) output += "\n" + new_indent;

  output += es::class_member("name()") + es::op("= ");
  output += es::identifier(R"(")") + es::type_name(name) + es::identifier(R"(")");

  _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;
}

}  // namespace _detail

}  // namespace cpp_dump
