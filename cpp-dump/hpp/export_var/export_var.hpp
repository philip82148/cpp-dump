/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#pragma once

#include <string>

#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "./export_arithmetic.hpp"
#include "./export_asterisk.hpp"
#include "./export_container.hpp"
#include "./export_enum.hpp"
#include "./export_exception.hpp"
#include "./export_map.hpp"
#include "./export_object.hpp"
#include "./export_object_generic.hpp"
#include "./export_ostream.hpp"
#include "./export_other/export_other.hpp"
#include "./export_pointer.hpp"
#include "./export_set.hpp"
#include "./export_string.hpp"
#include "./export_tuple.hpp"
#include "./export_unsupported.hpp"
#include "./export_xixo.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string export_var(
    [[maybe_unused]] const T &value,
    [[maybe_unused]] const std::string &indent,
    [[maybe_unused]] std::size_t last_line_length,
    [[maybe_unused]] std::size_t current_depth,
    [[maybe_unused]] bool fail_on_newline,
    [[maybe_unused]] const export_command &command
) {
  if constexpr (is_value_with_command<T>) {
    return export_var(
        value.value, indent, last_line_length, current_depth, fail_on_newline, value.command
    );
  } else if constexpr (is_exportable_object<T>) {
    return export_object(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_exportable_enum<T>) {
    return export_enum(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_arithmetic<T>) {
    return export_arithmetic(
        value, indent, last_line_length, current_depth, fail_on_newline, command
    );
  } else if constexpr (is_string<T>) {
    return export_string(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_map<T>) {
    return export_map(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_set<T>) {
    return export_set(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_container<T>) {
    return export_container(
        value, indent, last_line_length, current_depth, fail_on_newline, command
    );
  } else if constexpr (is_tuple<T>) {
    return export_tuple(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_xixo<T>) {
    return export_xixo(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_pointer<T>) {
    return export_pointer(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_exception<T>) {
    return export_exception(
        value, indent, last_line_length, current_depth, fail_on_newline, command
    );
  } else if constexpr (is_other_type<T>) {
    return export_other(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_ostream<T>) {
    return export_ostream(value, indent, last_line_length, current_depth, fail_on_newline, command);
  } else if constexpr (is_exportable_object_generic<T>) {
    return export_object_generic(
        value, indent, last_line_length, current_depth, fail_on_newline, command
    );
  } else if constexpr (is_asterisk<T>) {
    return export_asterisk(
        value, indent, last_line_length, current_depth, fail_on_newline, command
    );
  } else {
    static_assert(!is_exportable<T>, "is_exportable<T> has a bug! This should not be showed.");
    return export_unsupported();
  }
}

}  // namespace _detail

/**
 * Return a string representation of a variable.
 */
template <typename T>
std::string export_var(const T &value) {
  return _detail::export_var(value, "", 0, 0, false, _detail::export_command::default_command);
}

}  // namespace cpp_dump
