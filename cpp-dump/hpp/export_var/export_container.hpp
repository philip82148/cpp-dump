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
  // In case the container is empty.
  if (is_empty_iterable(container)) {
    return es::bracket("[ ]", current_depth);
  }
  // In case the depth exceeds max_depth.
  if (current_depth >= options::max_depth) {
    return es::bracket("[ ", current_depth) + es::op("...") + es::bracket(" ]", current_depth);
  }

  // Declare variables.
  std::size_t next_depth = current_depth + 1;
  const auto &next_command = command.next();
  auto skipped_container = command.create_skip_container(container);
  bool shift_indent = false;
  if (options::cont_indent_style == types::cont_indent_style_t::always) {
    shift_indent = true;
  } else if (options::cont_indent_style == types::cont_indent_style_t::when_nested) {
    shift_indent = is_iterable_like<iterable_elem_type<T>>;
  } else if (options::cont_indent_style == types::cont_indent_style_t::when_non_tuples_nested) {
    shift_indent = is_iterable_like<iterable_elem_type<T>> && !is_tuple<iterable_elem_type<T>>;
  }

  // Try printing on one line.
  if (!shift_indent) {
    std::string output = es::bracket("[ ", current_depth);
    bool is_first_elem = true;
    for (auto &&[is_ellipsis, it, index_] : skipped_container) {
      const auto &elem = *it;

      // Add comma.
      if (is_first_elem) {
        is_first_elem = false;
      } else {
        output += es::op(", ");
      }

      // If the `elem` is an ellipsis, skip it.
      if (is_ellipsis) {
        output += es::op("...");
        if (last_line_length + get_length(output) + std::string_view(" ]").size()
            > options::max_line_width) {
          shift_indent = true;
          break;
        }
        continue;
      }

      // Add the index if needed.
      if (command.show_index()) {
        output += es::member(std::to_string(index_)) + es::op(": ");
      }

      // Add the stringified `elem`.
      std::string elem_str = export_var(
          elem, indent, last_line_length + get_length(output), next_depth, true, next_command
      );
      if (has_newline(elem_str)) {
        shift_indent = true;
        break;
      }
      output += elem_str;

      // If the line length exceeds, stop the iteration.
      if (last_line_length + get_length(output) + std::string_view(" ]").size()
          > options::max_line_width) {
        shift_indent = true;
        break;
      }
    }

    if (!shift_indent) {
      output += es::bracket(" ]", current_depth);
      return output;
    }
  }

  // Print on multiple lines.

  if (fail_on_newline) {
    return "\n";
  }

  // Declare variables.
  std::string new_indent = indent + "  ";
  std::string output = es::bracket("[", current_depth);
  bool is_first_elem = true;

  // universal references; it.operator*() might not be const
  for (auto &&[is_ellipsis, it, index_] : skipped_container) {
    const auto &elem = *it;

    // Add comma.
    if (is_first_elem) {
      is_first_elem = false;
    } else {
      output += es::op(",");
    }

    // If the `elem` is an ellipsis, skip it.
    if (is_ellipsis) {
      output += "\n" + new_indent + es::op("...");
      continue;
    }

    output += "\n" + new_indent;

    // Add the index if needed.
    if (command.show_index()) {
      output += es::member(std::to_string(index_)) + es::op(": ");
    }

    // Add the stringified `elem`.
    output +=
        export_var(elem, new_indent, get_last_line_length(output), next_depth, false, next_command);
  }
  output += "\n" + indent + es::bracket("]", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
