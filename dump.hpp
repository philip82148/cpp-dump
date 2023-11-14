/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <utility>

#include "hpp/escape_sequence.hpp"
#include "hpp/expand_va_macro.hpp"
#include "hpp/export_command/export_command.hpp"
#include "hpp/export_var/export_var.hpp"
#include "hpp/log_label.hpp"
#include "hpp/options.hpp"
#include "hpp/utility.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_CPP_DUMP(expr) #expr, expr

/**
 * Print string representations of expression(s) and result(s) to std::clog.
 * This macro uses cpp_dump::export_var() internally.
 */
#define CPP_DUMP(...)                                                                              \
  cpp_dump::_detail::cpp_dump_macro(                                                               \
      {__FILE__, __LINE__, __func__},                                                              \
      _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_CPP_DUMP, __VA_ARGS__)                          \
  )

/**
 * Print string representations of expression(s) and result(s) to std::clog.
 * This macro uses cpp_dump::export_var() internally.
 * This is an alias of CPP_DUMP(...).
 */
#define cpp_dump(...) CPP_DUMP(__VA_ARGS__)

namespace cpp_dump {

namespace _detail {

template <typename T>
bool _dump_one(
    std::string &output,
    const std::string &log_label,
    bool no_newline_in_value_string,
    const std::string &expr,
    const T &value
) {
  const std::string initial_indent(get_last_line_length(log_label), ' ');
  const std::string second_indent = initial_indent + "  ";

  if (output.length() == 0) {
    output = es::log(log_label);
  } else {
    if (no_newline_in_value_string) {
      output += es::log(", ");
    } else {
      output += es::log(",\n") + initial_indent;
    }
  }

  struct prefix_and_value_string {
    std::string prefix;
    std::string value_string;
    bool value_string_has_newline;
    bool over_max_line_width;
  };

  auto make_prefix_and_value_string = [&, no_newline_in_value_string](
                                          const std::string &prefix, const std::string &indent
                                      ) -> prefix_and_value_string {
    auto last_line_length = get_last_line_length(output + prefix);

    std::string value_string = export_var(
        value,
        indent,
        last_line_length,
        0,
        no_newline_in_value_string,
        export_command::default_command
    );

    bool value_string_has_newline = has_newline(value_string);

    bool over_max_line_width =
        last_line_length + get_first_line_length(value_string) > max_line_width;

    return {prefix, value_string, value_string_has_newline, over_max_line_width};
  };

  auto append_output = [&](const prefix_and_value_string &pattern) -> void {
    output += pattern.prefix + pattern.value_string;
  };

  // for _dump_recursively_without_expr(), which is for cpp_dump::dump() (function)
  if (expr == "") {
    prefix_and_value_string pattern1 = make_prefix_and_value_string("", initial_indent);

    if (!no_newline_in_value_string) {
      append_output(pattern1);
      return true;
    }

    if (!pattern1.value_string_has_newline && !pattern1.over_max_line_width) {
      append_output(pattern1);
      return true;
    }

    if (get_last_line_length(output) <= initial_indent.length()) return false;

    prefix_and_value_string pattern2 =
        make_prefix_and_value_string("\n" + initial_indent, initial_indent);

    if (!pattern2.value_string_has_newline && !pattern2.over_max_line_width) {
      append_output(pattern2);
      return true;
    }

    return false;
  }

  // below for _dump_recursively_with_expr(), which is for cpp_dump() (macro)
  auto expr_with_es = es::expression(expr);

  if (no_newline_in_value_string) {
    prefix_and_value_string pattern1a =
        make_prefix_and_value_string(expr_with_es + es::log(" => "), initial_indent);

    if (!pattern1a.value_string_has_newline && !pattern1a.over_max_line_width) {
      append_output(pattern1a);
      return true;
    }

    if (get_last_line_length(output) <= initial_indent.length()) {
      prefix_and_value_string pattern1b = make_prefix_and_value_string(
          expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
      );

      if (!pattern1b.value_string_has_newline) {
        append_output(pattern1b);
        return true;
      }

      return false;
    }

    prefix_and_value_string pattern2a = make_prefix_and_value_string(
        "\n" + initial_indent + expr_with_es + es::log(" => "), initial_indent
    );

    if (!pattern2a.value_string_has_newline && !pattern2a.over_max_line_width) {
      append_output(pattern2a);
      return true;
    }

    prefix_and_value_string pattern2b = make_prefix_and_value_string(
        "\n" + initial_indent + expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
    );

    if (!pattern2b.value_string_has_newline) {
      append_output(pattern2b);
      return true;
    }

    return false;
  }

  prefix_and_value_string pattern1a =
      make_prefix_and_value_string(expr_with_es + es::log(" => "), initial_indent);

  if (!pattern1a.over_max_line_width) {
    if (!pattern1a.value_string_has_newline) {
      append_output(pattern1a);
      return true;
    }

    prefix_and_value_string pattern1b = make_prefix_and_value_string(
        expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
    );

    if (!pattern1b.value_string_has_newline) {
      append_output(pattern1b);
      return true;
    }

    append_output(pattern1a);
    return true;
  }

  prefix_and_value_string pattern1b = make_prefix_and_value_string(
      expr_with_es + "\n" + second_indent + es::log("=> "), second_indent
  );

  append_output(pattern1b);
  return true;
}

inline bool _dump_recursively_with_expr(std::string &, const std::string &, bool) { return true; }

template <typename T, typename... Args>
inline bool _dump_recursively_with_expr(
    std::string &output,
    const std::string &log_label,
    bool no_newline_in_value_string,
    const std::string &expr,
    const T &value,
    const Args &...args
) {
  return _dump_one(output, log_label, no_newline_in_value_string, expr, value)
         && _dump_recursively_with_expr(output, log_label, no_newline_in_value_string, args...);
}

inline bool _dump_recursively_without_expr(std::string &, const std::string &, bool) {
  return true;
}

template <typename T, typename... Args>
inline bool _dump_recursively_without_expr(
    std::string &output,
    const std::string &log_label,
    bool no_newline_in_value_string,
    const T &value,
    const Args &...args
) {
  return _dump_one(output, log_label, no_newline_in_value_string, "", value)
         && _dump_recursively_without_expr(output, log_label, no_newline_in_value_string, args...);
}

struct _source_location {
  std::string file_name;
  std::size_t line;
  std::string function_name;
};

// function called by cpp_dump() macro
template <typename... Args>
void cpp_dump_macro(_source_location loc, const Args &...args) {
  std::string log_label =
      log_label_func ? log_label_func(loc.file_name, loc.line, loc.function_name) : "";

  std::string output = "";
  if (!_detail::_dump_recursively_with_expr(output, log_label, true, args...)) {
    output = "";
    _detail::_dump_recursively_with_expr(output, log_label, false, args...);
  }

  std::clog << output << std::endl;
}

}  // namespace _detail

/**
 * Print string representation(s) of variable(s) to std::clog.
 * This function uses cpp_dump::export_var() internally.
 */
template <typename... Args>
void dump(const Args &...args) {
  std::string log_label = log_label_func ? log_label_func("", 0, "") : "";

  std::string output = "";
  if (!_detail::_dump_recursively_without_expr(output, log_label, true, args...)) {
    output = "";
    _detail::_dump_recursively_without_expr(output, log_label, false, args...);
  }

  std::clog << output << std::endl;
}

}  // namespace cpp_dump
