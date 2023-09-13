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

#include "hpp/expand_va_macro.hpp"
#include "hpp/export_var.hpp"
#include "hpp/utility.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_CPP_DUMP(expr) #expr, expr

/**
 * Output string representation of expression(s) and the result(s) to std::clog.
 * This function uses cpp_dump::export_var() internally.
 */
#define cpp_dump(...)                                                                              \
  cpp_dump::_detail::cpp_dump_(_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_CPP_DUMP, __VA_ARGS__))

/**
 * Set a value to a variable in cpp_dump namespace.
 */
#define CPP_DUMP_SET_OPTION(variable, value) cpp_dump::variable = value

namespace cpp_dump {

/**
 * Maximum line width of a output string of cpp_dump::export_var().
 */
inline size_t max_line_width = 160;

/**
 * Maximum number of times cpp_dump::export_var() is applied recursively.
 */
inline size_t max_depth = 5;

/**
 * Maximum number of times cpp_dump::export_var() iterates over an iterator.
 */
inline size_t max_iteration_count = 100;

namespace _detail {

template <typename T>
bool _dump_one(
    std::string &output, bool no_newline_in_value_string, const std::string &expr, const T &value
) {
  const std::string indent7 = "       ";
  const std::string indent9 = "         ";

  if (output.length() == 0) {
    output = "[dump] ";
  } else {
    if (no_newline_in_value_string) {
      output += ", ";
    } else {
      output += ",\n" + indent7;
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
    prefix_and_value_string pattern1 = make_prefix_and_value_string("", indent7);

    if (!no_newline_in_value_string) {
      append_output(pattern1);
      return true;
    }

    if (!pattern1.value_string_has_newline && !pattern1.over_max_line_width) {
      append_output(pattern1);
      return true;
    }

    if (get_last_line_length(output) <= indent7.length()) return false;

    prefix_and_value_string pattern2 = make_prefix_and_value_string("\n" + indent7, indent7);

    if (!pattern2.value_string_has_newline && !pattern2.over_max_line_width) {
      append_output(pattern2);
      return true;
    }

    return false;
  }

  // below for dump_recursive_with_expr(), which is for cpp_dump() (macro)

  if (no_newline_in_value_string) {
    prefix_and_value_string pattern1a = make_prefix_and_value_string(expr + " => ", indent7);

    if (!pattern1a.value_string_has_newline && !pattern1a.over_max_line_width) {
      append_output(pattern1a);
      return true;
    }

    if (get_last_line_length(output) <= indent7.length()) {
      prefix_and_value_string pattern1b =
          make_prefix_and_value_string(expr + "\n" + indent9 + "=> ", indent9);

      if (!pattern1b.value_string_has_newline) {
        append_output(pattern1b);
        return true;
      }

      return false;
    }

    prefix_and_value_string pattern2a =
        make_prefix_and_value_string("\n" + indent7 + expr + " => ", indent7);

    if (!pattern2a.value_string_has_newline && !pattern2a.over_max_line_width) {
      append_output(pattern2a);
      return true;
    }

    prefix_and_value_string pattern2b =
        make_prefix_and_value_string("\n" + indent7 + expr + "\n" + indent9 + "=> ", indent9);

    if (!pattern2b.value_string_has_newline) {
      append_output(pattern2b);
      return true;
    }

    return false;
  }

  prefix_and_value_string pattern1a = make_prefix_and_value_string(expr + " => ", indent7);

  if (!pattern1a.over_max_line_width) {
    if (!pattern1a.value_string_has_newline) {
      append_output(pattern1a);
      return true;
    }

    prefix_and_value_string pattern1b =
        make_prefix_and_value_string(expr + "\n" + indent9 + "=> ", indent9);

    if (!pattern1b.value_string_has_newline) {
      append_output(pattern1b);
      return true;
    }

    append_output(pattern1a);
    return true;
  }

  prefix_and_value_string pattern1b =
      make_prefix_and_value_string(expr + "\n" + indent9 + "=> ", indent9);

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
  return _dump_one(output, no_newline_in_value_string, expr, value)
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

// function called by cpp_dump() macro
template <typename... Args>
void cpp_dump_(const Args &...args) {
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
 * Output string representation of variable(s) to std::clog.
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
