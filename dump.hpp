/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "hpp/escape_sequence.hpp"
#include "hpp/expand_va_macro.hpp"
#include "hpp/export_var.hpp"
#include "hpp/utility.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_CPP_DUMP(expr) #expr, expr

/**
 * Print string representations of expression(s) and result(s) to std::clog.
 * This function uses cpp_dump::export_var() internally.
 */
#define CPP_DUMP(...)                                                                              \
  cpp_dump::_detail::cpp_dump_macro(                                                               \
      _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_CPP_DUMP, __VA_ARGS__)                          \
  )

/**
 * Set a value to a variable in cpp_dump namespace.
 */
#define CPP_DUMP_SET_OPTION(variable, value) cpp_dump::variable = value

namespace cpp_dump {

/**
 * Maximum line width of output strings of cpp_dump::export_var().
 */
inline size_t max_line_width = 160;

/**
 * Maximum number of times cpp_dump::export_var() is applied recursively.
 */
inline size_t max_depth = 5;

/**
 * Maximum number of times cpp_dump::export_var() iterates over an iterator.
 * Note that in a single call, export_var() calls itself at most
 * (max_iteration_count^max_depth-1)/(max_iteration_count-1)-1 times.
 */
inline size_t max_iteration_count = 16;

/**
 * Label that cpp_dump::dump() and CPP_DUMP() print at the beginning of the output.
 */
inline std::string log_label = "[dump] ";

inline use_es use_es_value = use_es::enhanced;

inline escape_sequence es_value = es_theme::rich;

namespace _detail {

template <typename T>
bool _dump_one(
    std::string &output, bool no_newline_in_value_string, const std::string &expr, const T &value
) {
  const std::string initial_indent = ([] {
    std::string indent;
    for (auto _ : log_label) indent += " ";
    return indent;
  })();
  const std::string second_indent  = initial_indent + "  ";

  if (output.length() == 0) {
    output = log_label;
  } else {
    if (no_newline_in_value_string) {
      output += ", ";
    } else {
      output += ",\n" + initial_indent;
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

    std::string value_string =
        export_var(value, indent, last_line_length, 1, no_newline_in_value_string);

    bool value_string_has_newline = has_newline(value_string);

    bool over_max_line_width =
        last_line_length + get_first_line_length(value_string) > max_line_width;

    return {prefix, value_string, value_string_has_newline, over_max_line_width};
  };

  auto append_output = [&](const prefix_and_value_string &pattern) -> void {
    output += pattern.prefix + pattern.value_string;
  };

  // for dump_recursive_without_expr(), which is for cpp_dump::dump() (function)
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

  // below for dump_recursive_with_expr(), which is for CPP_DUMP() (macro)

  if (no_newline_in_value_string) {
    prefix_and_value_string pattern1a = make_prefix_and_value_string(expr + " => ", initial_indent);

    if (!pattern1a.value_string_has_newline && !pattern1a.over_max_line_width) {
      append_output(pattern1a);
      return true;
    }

    if (get_last_line_length(output) <= initial_indent.length()) {
      prefix_and_value_string pattern1b =
          make_prefix_and_value_string(expr + "\n" + second_indent + "=> ", second_indent);

      if (!pattern1b.value_string_has_newline) {
        append_output(pattern1b);
        return true;
      }

      return false;
    }

    prefix_and_value_string pattern2a =
        make_prefix_and_value_string("\n" + initial_indent + expr + " => ", initial_indent);

    if (!pattern2a.value_string_has_newline && !pattern2a.over_max_line_width) {
      append_output(pattern2a);
      return true;
    }

    prefix_and_value_string pattern2b = make_prefix_and_value_string(
        "\n" + initial_indent + expr + "\n" + second_indent + "=> ", second_indent
    );

    if (!pattern2b.value_string_has_newline) {
      append_output(pattern2b);
      return true;
    }

    return false;
  }

  prefix_and_value_string pattern1a = make_prefix_and_value_string(expr + " => ", initial_indent);

  if (!pattern1a.over_max_line_width) {
    if (!pattern1a.value_string_has_newline) {
      append_output(pattern1a);
      return true;
    }

    prefix_and_value_string pattern1b =
        make_prefix_and_value_string(expr + "\n" + second_indent + "=> ", second_indent);

    if (!pattern1b.value_string_has_newline) {
      append_output(pattern1b);
      return true;
    }

    append_output(pattern1a);
    return true;
  }

  prefix_and_value_string pattern1b =
      make_prefix_and_value_string(expr + "\n" + second_indent + "=> ", second_indent);

  append_output(pattern1b);
  return true;
}

inline bool _dump_recursive_with_expr(std::string &, bool) { return true; }

template <typename T, typename... Args>
inline bool _dump_recursive_with_expr(
    std::string &output,
    bool no_newline_in_value_string,
    const std::string &expr,
    const T &value,
    const Args &...args
) {
  return _dump_one(output, no_newline_in_value_string, with_es::expression(expr), value)
         && _dump_recursive_with_expr(output, no_newline_in_value_string, args...);
}

inline bool _dump_recursive_without_expr(std::string &, bool) { return true; }

template <typename T, typename... Args>
inline bool _dump_recursive_without_expr(
    std::string &output, bool no_newline_in_value_string, const T &value, const Args &...args
) {
  return _dump_one(output, no_newline_in_value_string, "", value)
         && _dump_recursive_without_expr(output, no_newline_in_value_string, args...);
}

// function called by CPP_DUMP() macro
template <typename... Args>
void cpp_dump_macro(const Args &...args) {
  bool no_newline_in_value_string = true;

rollback:
  std::string output = "";
  if (!_dump_recursive_with_expr(output, no_newline_in_value_string, args...)) {
    no_newline_in_value_string = false;
    goto rollback;
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
  bool no_newline_in_value_string = true;

rollback:
  std::string output = "";
  if (!_detail::_dump_recursive_without_expr(output, no_newline_in_value_string, args...)) {
    no_newline_in_value_string = false;
    goto rollback;
  }

  std::clog << output << std::endl;
}

}  // namespace cpp_dump
