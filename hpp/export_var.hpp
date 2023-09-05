/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./export_arithmetic.hpp"
#include "./export_bitset.hpp"
#include "./export_container.hpp"
#include "./export_enum.hpp"
#include "./export_map.hpp"
#include "./export_object.hpp"
#include "./export_pointer.hpp"
#include "./export_set.hpp"
#include "./export_string.hpp"
#include "./export_tuple_like.hpp"
#include "./export_xixo.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string export_var(const T &value, const std::string &indent, size_t last_line_length,
                       size_t current_depth, bool fail_on_newline) {
  static_assert(is_exportable<T>, "The type is not supported.");
  if constexpr (!is_exportable<T>) {
    static_assert(
        !(std::is_class_v<T> || std::is_union_v<T>),
        "Please use CPP_DUMP_DEFINE_EXPORT_OBJECT(type, members...) macro to support the type.");

    static_assert(
        !std::is_enum_v<T>,
        "Please use CPP_DUMP_DEFINE_EXPORT_ENUM(type, members...) macro to support the type.");
  }

  if constexpr (is_exportable_object<T>) {
    return export_object(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_arithmetic<T>) {
    return export_arithmetic(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_string<T>) {
    return export_string(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_pointer<T>) {
    return export_pointer(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_map<T>) {
    return export_map(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_set<T>) {
    return export_set(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_container<T>) {
    return export_container(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_tuple_like<T>) {
    return export_tuple_like(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_xixo<T>) {
    return export_xixo(value, indent, last_line_length, current_depth, fail_on_newline);
  } else if constexpr (is_bitset<T>) {
    return export_bitset(value, indent, last_line_length, current_depth, fail_on_newline);
  } else {
    return export_enum(value, indent, last_line_length, current_depth, fail_on_newline);
  }
}

}  // namespace _detail

template <typename T>
std::string export_var(const T &value) {
  return _detail::export_var(value, "", 0, 1, false);
}

}  // namespace cpp_dump
