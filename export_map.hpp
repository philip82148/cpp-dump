/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

extern inline int max_line_width;

template <typename T>
std::string export_var(const T &, const std::string &, size_t, bool);

template <typename T>
auto export_map(const T &value, const std::string &indent, size_t last_line_length,
                bool fail_on_newline) -> std::enable_if_t<is_map<T>, std::string> {
  if (value.empty()) return "{ }";

  bool shift_indent = is_iterable_like<map_key_type<T>> || is_iterable_like<map_value_type<T>>;
  // 中身がiterable_likeでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

rollback:
  std::string output = "{ ";
  bool is_first = true;
  for (auto elem_pair : value) {
    if (is_first) {
      is_first = false;
    } else {
      output += ", ";
    }

    if (shift_indent) {
      std::string key_string =
          "\n" + new_indent +
          export_var(elem_pair.first, new_indent, new_indent.length(), fail_on_newline) + ": ";
      std::string value_string =
          export_var(elem_pair.second, new_indent, _last_line_length(key_string), fail_on_newline);

      output += key_string + value_string;
      continue;
    }

    std::string key_string =
        export_var(elem_pair.first, indent, last_line_length + output.length(), fail_on_newline) +
        ": ";
    std::string value_string =
        export_var(elem_pair.second, indent,
                   last_line_length + output.length() + key_string.length(), fail_on_newline);

    std::string elem_string = key_string + value_string;
    if (!_has_newline(elem_string)) {
      output += elem_string;

      if (last_line_length + (output + " }").length() <= max_line_width) continue;
    }

    if (fail_on_newline) return "\n";

    shift_indent = true;
    goto rollback;
  }

  if (shift_indent) {
    output += "\n" + indent + "}";
  } else {
    output += " }";
  }

  return output;
}

}  // namespace cpp_dump
