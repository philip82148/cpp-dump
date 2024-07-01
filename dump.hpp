/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

#include "hpp/escape_sequence.hpp"
#include "hpp/expand_va_macro.hpp"
#include "hpp/export_command/export_command.hpp"
#include "hpp/export_var/export_var.hpp"
#include "hpp/options.hpp"
#include "hpp/utility.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_CPP_DUMP(expr)  #expr
#define _p_CPP_DUMP_EXPAND_FOR_CPP_DUMP2(expr) (expr)

/**
 * Print string representations of expression(s) and result(s) to std::clog.
 * This macro uses cpp_dump::export_var() internally.
 */
#define CPP_DUMP(...)                                                                              \
  cpp_dump::_detail::cpp_dump_macro(                                                               \
      {__FILE__, __LINE__, __func__},                                                              \
      {_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_CPP_DUMP, __VA_ARGS__)},                       \
      _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_CPP_DUMP2, __VA_ARGS__)                         \
  )

/**
 * Print string representations of expression(s) and result(s) to std::clog.
 * This macro uses cpp_dump::export_var() internally.
 * This is an alias of CPP_DUMP(...).
 */
#define cpp_dump(...) CPP_DUMP(__VA_ARGS__)

namespace cpp_dump {

/**
 * cpp_dump() uses this function to print logs.
 * Define an explicit specialization with 'void' to customize this function.
 */
template <typename = void>
void write_log(std::string_view output) {
  std::clog << output << std::endl;
}

namespace _detail {

template <typename T>
bool _dump_one(
    std::string &output,
    const std::string &log_label,
    bool always_newline_before_expr,
    std::string_view expr,
    const T &value
) {
  const std::string initial_indent(get_last_line_length(log_label), ' ');
  const std::string second_indent = initial_indent + "  ";
  const bool fail_on_newline_in_value = !always_newline_before_expr;

  if (output.length() == 0) {
    output = es::reset() + es::log(log_label);
  } else {
    if (always_newline_before_expr) {
      output += es::log(",\n") + initial_indent;
    } else {
      output += es::log(", ");
    }
  }

  struct prefix_and_value_str {
    std::string prefix;
    std::string value_str;
    bool value_str_has_newline;
    bool over_max_line_width;
  };

  auto make_prefix_and_value_str = [&, fail_on_newline_in_value](
                                       const std::string &prefix, const std::string &indent
                                   ) -> prefix_and_value_str {
    auto last_line_length = get_last_line_length(output + prefix);

    std::string value_str = export_var(
        value,
        indent,
        last_line_length,
        0,
        fail_on_newline_in_value,
        export_command::default_command
    );

    bool value_str_has_newline = has_newline(value_str);

    bool over_max_line_width = last_line_length + get_first_line_length(value_str) > max_line_width;

    return {prefix, value_str, value_str_has_newline, over_max_line_width};
  };

  auto append_output = [&](const prefix_and_value_str &pattern) -> void {
    output += pattern.prefix + pattern.value_str;
  };

  if (!print_expr) {
    prefix_and_value_str pattern1 = make_prefix_and_value_str("", initial_indent);

    if (!fail_on_newline_in_value) {
      append_output(pattern1);
      return true;
    }

    if (!(pattern1.value_str_has_newline || pattern1.over_max_line_width)) {
      append_output(pattern1);
      return true;
    }

    if (get_last_line_length(output) <= initial_indent.length()) return false;

    prefix_and_value_str pattern2 =
        make_prefix_and_value_str("\n" + initial_indent, initial_indent);

    if (!(pattern2.value_str_has_newline || pattern2.over_max_line_width)) {
      append_output(pattern2);
      return true;
    }

    return false;
  }

  auto expr_with_es = es::expression(expr);

  if (fail_on_newline_in_value) {
    prefix_and_value_str pattern1a =
        make_prefix_and_value_str(expr_with_es + es::log(" => "), initial_indent);

    if (!(pattern1a.value_str_has_newline || pattern1a.over_max_line_width)) {
      append_output(pattern1a);
      return true;
    }

    if (get_last_line_length(output) <= initial_indent.length()) {
      prefix_and_value_str pattern1b = make_prefix_and_value_str(
          expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
      );

      if (!pattern1b.value_str_has_newline) {
        append_output(pattern1b);
        return true;
      }

      return false;
    }

    prefix_and_value_str pattern2a = make_prefix_and_value_str(
        "\n" + initial_indent + expr_with_es + es::log(" => "), initial_indent
    );

    if (!(pattern2a.value_str_has_newline || pattern2a.over_max_line_width)) {
      append_output(pattern2a);
      return true;
    }

    prefix_and_value_str pattern2b = make_prefix_and_value_str(
        "\n" + initial_indent + expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
    );

    if (!pattern2b.value_str_has_newline) {
      append_output(pattern2b);
      return true;
    }

    return false;
  }

  prefix_and_value_str pattern1a =
      make_prefix_and_value_str(expr_with_es + es::log(" => "), initial_indent);

  if (!pattern1a.over_max_line_width) {
    if (!pattern1a.value_str_has_newline) {
      append_output(pattern1a);
      return true;
    }

    prefix_and_value_str pattern1b = make_prefix_and_value_str(
        expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
    );

    if (!pattern1b.value_str_has_newline) {
      append_output(pattern1b);
      return true;
    }

    append_output(pattern1a);
    return true;
  }

  prefix_and_value_str pattern1b = make_prefix_and_value_str(
      expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
  );

  append_output(pattern1b);
  return true;
}

template <typename... Args>
bool _dump(
    std::string &output,
    const std::string &log_label,
    bool always_newline_before_expr,
    const std::initializer_list<std::string_view> &exprs,
    const Args &...args
) {
  auto it = exprs.begin();
  return (... && _dump_one(output, log_label, always_newline_before_expr, *it++, args));
}

struct _source_location {
  std::string_view file_name;
  std::size_t line;
  std::string_view function_name;
};

// function called by cpp_dump() macro
template <typename... Args>
void cpp_dump_macro(
    _source_location loc, std::initializer_list<std::string_view> exprs, const Args &...args
) {
  std::string log_label =
      log_label_func ? log_label_func(loc.file_name, loc.line, loc.function_name) : "";

  bool exprs_have_newline = print_expr && std::any_of(exprs.begin(), exprs.end(), has_newline);

  std::string output;
  if (exprs_have_newline || !_dump(output, log_label, false, exprs, args...)) {
    output = "";
    _dump(output, log_label, true, exprs, args...);
  }

  write_log(output);
}

}  // namespace _detail

}  // namespace cpp_dump
