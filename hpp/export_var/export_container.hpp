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
#include "../iterable.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"

namespace cpp_dump {

extern inline std::size_t max_line_width;

extern inline std::size_t max_depth;

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

template <typename T>
inline auto export_container(
    const T &container,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_container<T>, std::string> {
  if (is_empty_iterable(container)) return es::bracket("[ ]", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("[ ", current_depth) + es::op("...") + es::bracket(" ]", current_depth);

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  // 中身がiterable_likeでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  std::size_t next_depth = current_depth + 1;
  auto next_command      = command.next();

  auto omitted = command.get_omitted_container(container);

rollback:
  std::string output = es::bracket("[ ", current_depth);
  bool is_first      = true;

  for (const auto &[skip, it] : omitted) {
    const auto &elem = *it;

    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) {
      if (skip) {
        output += "\n" + new_indent + es::op("...");
        continue;
      }

      output +=
          "\n" + new_indent
          + export_var(elem, new_indent, new_indent.length(), next_depth, false, next_command);
      continue;
    }

    if (skip) {
      output += es::op("...");

      if (last_line_length + get_length(output + " ]") <= max_line_width) continue;

      shift_indent = true;
      goto rollback;
    }

    std::string elem_string = export_var(
        elem, indent, last_line_length + get_length(output), next_depth, true, next_command
    );
    if (!has_newline(elem_string)) {
      output += elem_string;

      if (last_line_length + get_length(output + " ]") <= max_line_width) continue;
    }

    if (fail_on_newline) return "\n";

    shift_indent = true;
    goto rollback;
  }

  if (shift_indent) {
    output += "\n" + indent + es::bracket("]", current_depth);
  } else {
    output += es::bracket(" ]", current_depth);
  }

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
