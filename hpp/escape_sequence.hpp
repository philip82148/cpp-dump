/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "./utility.hpp"

namespace cpp_dump {

/**
 * Type of cpp_dump::es_style.
 * cpp_dump::export_var() supports this type.
 */
enum class es_style_t { no_es, by_syntax };

/**
 * Type of cpp_dump::es_value.
 * cpp_dump::export_var() supports this type.
 */
struct es_value_t {
 public:
  std::string log;
  std::string expression;
  std::vector<std::string> bracket_by_depth;
  std::string reserved;
  std::string number;
  std::string character;
  std::string op;
  std::string identifier;
  std::string member;
  std::string unsupported;
  es_value_t(
      std::string log,
      std::string expression,
      std::vector<std::string> bracket_by_depth,
      std::string reserved,
      std::string number,
      std::string character,
      std::string op,
      std::string identifier,
      std::string member,
      std::string unsupported
  )
      : log(log),
        expression(expression),
        bracket_by_depth(bracket_by_depth),
        reserved(reserved),
        number(number),
        character(character),
        op(op),
        identifier(identifier),
        member(member),
        unsupported(unsupported) {}
};

extern inline es_style_t es_style;

extern inline es_value_t es_value;

extern inline size_t max_line_width;

extern inline size_t max_depth;

extern inline size_t max_iteration_count;

namespace _detail {

inline bool use_es() { return es_style != es_style_t::no_es; }

namespace es {

inline std::string apply(const std::string &es, const std::string &s) {
  if (use_es()) {
    const std::string reset = "\e[0m";
    return es + s + reset;
  } else {
    return s;
  }
}

inline std::string log(const std::string &s) { return es::apply(es_value.log, s); }
inline std::string expression(const std::string &s) { return es::apply(es_value.expression, s); }
inline std::string reserved(const std::string &s) { return es::apply(es_value.reserved, s); }
inline std::string number(const std::string &s) { return es::apply(es_value.number, s); }
inline std::string character(const std::string &s) { return es::apply(es_value.character, s); }
inline std::string op(const std::string &s) { return es::apply(es_value.op, s); }
inline std::string identifier(const std::string &s) { return es::apply(es_value.identifier, s); }
inline std::string member(const std::string &s) { return es::apply(es_value.member, s); }
inline std::string unsupported(const std::string &s) { return es::apply(es_value.unsupported, s); }

inline std::string bracket(const std::string &s, size_t d) {
  auto sz = es_value.bracket_by_depth.size();
  if (sz == 0) return s;

  return es::apply(es_value.bracket_by_depth[d % sz], s);
}

}  // namespace es

inline std::string _export_es_value_string(
    const std::string &es, const std::string &, size_t, size_t, bool
) {
  std::string escaped_es = es;
  replace_string(escaped_es, "\e", "\\e");

  return es::apply(es, R"(")" + escaped_es + R"(")");
}

inline std::string _export_es_value_vector(
    const std::vector<std::string> &es_vec,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  if (es_vec.empty()) return es::bracket("[ ]", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("[ ", current_depth) + es::op("...") + es::bracket(" ]", current_depth);

  bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  size_t next_depth      = current_depth + 1;

rollback:
  std::string output     = es::bracket("[ ", current_depth);
  bool is_first          = true;
  size_t iteration_count = 0;
  for (const auto &es : es_vec) {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) {
      if (++iteration_count > max_iteration_count) {
        output += "\n" + new_indent + es::op("...");
        break;
      }

      output += "\n" + new_indent
                + _export_es_value_string(es, new_indent, new_indent.length(), next_depth, false);
      continue;
    }

    if (++iteration_count > max_iteration_count) {
      output += es::op("...");

      if (last_line_length + get_length(output + " ]") <= max_line_width) break;

      shift_indent = true;
      goto rollback;
    }

    output += _export_es_value_string(
        es, indent, last_line_length + get_length(output), next_depth, true
    );

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
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) {
  if (current_depth >= max_depth)
    return es::identifier("cpp_dump::es_value_t") + es::bracket("{ ", current_depth) + es::op("...")
           + es::bracket(" }", current_depth);

  std::string new_indent = indent + "  ";
  size_t next_depth      = current_depth + 1;

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

    if constexpr (std::is_convertible_v<decltype(member), std::string>) {
      output += es::apply(member, member_name + "= ");
      output += _export_es_value_string(
          member, new_indent, get_last_line_length(output), next_depth, false
      );
    } else {
      output += es::member(member_name) + es::op("= ");
      output += _export_es_value_vector(
          member, new_indent, get_last_line_length(output), next_depth, false
      );
    }
  };

rollback:
  output   = es::identifier("cpp_dump::es_value_t") + es::bracket("{ ", current_depth);
  is_first = true;

  append_output("log", esv.log);
  append_output("expression", esv.expression);
  append_output("bracket_by_depth", esv.bracket_by_depth);
  append_output("reserved", esv.reserved);
  append_output("character", esv.character);
  append_output("op", esv.op);
  append_output("identifier", esv.identifier);
  append_output("member", esv.member);
  append_output("unsupported", esv.unsupported);

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