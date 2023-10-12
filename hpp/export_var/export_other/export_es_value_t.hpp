/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <vector>

#include "../../escape_sequence.hpp"
#include "../../export_command/export_command.hpp"
#include "../../utility.hpp"

namespace cpp_dump {

extern inline std::size_t max_line_width;

extern inline std::size_t max_depth;

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

inline std::string _export_es_value_string(const std::string &es) {
  std::string escaped_es = es;
  replace_string(escaped_es, "\e", "\\e");

  return es::apply(es, R"(")" + escaped_es + R"(")");
}

inline std::string _export_es_value_vector(
    const std::vector<std::string> &es_vec,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  if (es_vec.empty()) return es::bracket("[ ]", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("[ ", current_depth) + es::op("...") + es::bracket(" ]", current_depth);

  bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  auto omitted = command.get_omitted_container(es_vec);

rollback:
  std::string output = es::bracket("[ ", current_depth);
  bool is_first      = true;

  for (const auto &[is_ellipsis, es] : omitted) {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) {
      if (is_ellipsis) {
        output += "\n" + new_indent + es::op("...");
        continue;
      }

      output += "\n" + new_indent + _export_es_value_string(es);
      continue;
    }

    if (is_ellipsis) {
      output += es::op("...");

      if (last_line_length + get_length(output + " ]") <= max_line_width) continue;

      shift_indent = true;
      goto rollback;
    }

    output += _export_es_value_string(es);

    if (last_line_length + get_length(output + " ]") <= max_line_width) continue;

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

inline std::string export_es_value_t(
    const es_value_t &esv,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  if (current_depth >= max_depth)
    return es::identifier("cpp_dump::es_value_t") + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  std::string new_indent = indent + "  ";
  std::size_t next_depth = current_depth + 1;

  bool shift_indent = false;

  std::string output;
  bool is_first;

  auto append_output = [&](const std::string &member_name, const auto &member) -> void {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) output += "\n" + new_indent;

    if constexpr (std::is_same_v<decltype(member), const std::string &>) {
      output += es::apply(member, member_name + "= ");
      output += _export_es_value_string(member);
    } else {
      output += es::member(member_name) + es::op("= ");
      output += _export_es_value_vector(
          member, new_indent, get_last_line_length(output), next_depth, false, command
      );
    }
  };

rollback:
  output   = es::identifier("cpp_dump::es_value_t") + es::bracket("{ ", current_depth);
  is_first = true;

  append_output("log", esv.log);
  append_output("expression", esv.expression);
  append_output("reserved", esv.reserved);
  append_output("character", esv.character);
  append_output("op", esv.op);
  append_output("identifier", esv.identifier);
  append_output("member", esv.member);
  append_output("unsupported", esv.unsupported);
  append_output("bracket_by_depth", esv.bracket_by_depth);

  if (!shift_indent) {
    output += es::bracket(" }", current_depth);

    if (!has_newline(output) && last_line_length + get_length(output) <= max_line_width)
      return output;

    if (fail_on_newline) return "\n";

    shift_indent = true;
    goto rollback;
  }

  output += "\n" + indent + es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump