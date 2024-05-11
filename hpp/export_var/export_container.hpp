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
#include "../iterable.hpp"
#include "../options.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"
#include "./export_var_fwd.hpp"

namespace cpp_dump {

namespace _detail {

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

  std::size_t next_depth = current_depth + 1;
  const auto &next_command = command.next();
  auto skipped_container = command.create_skip_container(container);

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  if (!shift_indent) {
    std::string output = es::bracket("[ ", current_depth);
    bool is_first_elem = true;

    for (auto &&[skip, it, index] : skipped_container) {
      const auto &elem = *it;

      if (is_first_elem) {
        is_first_elem = false;
      } else {
        output += es::op(", ");
      }

      if (skip) {
        output += es::op("...");

        if (last_line_length + get_length(output) + std::string_view(" ]").size()
            > max_line_width) {
          shift_indent = true;
          break;
        }

        continue;
      }

      if (command.show_index()) output += es::member(std::to_string(index)) + es::op(": ");

      std::string elem_str = export_var(
          elem, indent, last_line_length + get_length(output), next_depth, true, next_command
      );

      if (has_newline(elem_str)) {
        shift_indent = true;
        break;
      }

      output += elem_str;
      if (last_line_length + get_length(output) + std::string_view(" ]").size() > max_line_width) {
        shift_indent = true;
        break;
      }
    }

    if (!shift_indent) {
      output += es::bracket(" ]", current_depth);

      return output;
    }
  }

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  std::string output = es::bracket("[", current_depth);
  bool is_first_elem = true;

  for (auto &&[skip, it, index] : skipped_container) {
    const auto &elem = *it;

    if (is_first_elem) {
      is_first_elem = false;
    } else {
      output += es::op(",");
    }

    if (skip) {
      output += "\n" + new_indent + es::op("...");
      continue;
    }

    output += "\n" + new_indent;
    if (command.show_index()) output += es::member(std::to_string(index)) + es::op(": ");

    output +=
        export_var(elem, new_indent, get_last_line_length(output), next_depth, false, next_command);
  }
  output += "\n" + indent + es::bracket("]", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
