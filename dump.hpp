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

#include "./define_export_object_macro.hpp"
#include "./expand_va_macro.hpp"
#include "./export_var.hpp"
#include "./utility.hpp"

#define CPP_DUMP_EXPAND_EXPR_FOR_DUMP_(expr) #expr, (expr)
#define dump(...) cpp_dump::_dump(CPP_DUMP_EXPAND_VA_(CPP_DUMP_EXPAND_EXPR_FOR_DUMP_, __VA_ARGS__))

namespace cpp_dump {

inline int max_line_width = 160;

template <typename T>
bool _dump_one(std::string &output, bool no_newline_in_value_string, const std::string &expr,
               const T &value) {
  const std::string indent9 = "         ";
  const std::string indent11 = "           ";

  if (output.length() == 0) {
    output = "[dump()] ";
  } else {
    if (no_newline_in_value_string) {
      output += ", ";
    } else {
      output += ",\n" + indent9;
    }
  }

  struct prefix_and_value_string {
    std::string prefix;
    std::string value_string;
    bool value_string_has_newline;
    bool over_max_line_width;
  };

  auto get_prefix_and_value_string = [&, no_newline_in_value_string](
                                         const std::string &prefix,
                                         const std::string &indent) -> prefix_and_value_string {
    auto last_line_length = _last_line_length(output + prefix);

    std::string value_string =
        export_var(value, indent, last_line_length, no_newline_in_value_string);

    bool value_string_has_newline = _has_newline(value_string);

    bool over_max_line_width = last_line_length + _first_line_length(value_string) > max_line_width;

    return {prefix, value_string, value_string_has_newline, over_max_line_width};
  };

  auto append_output = [&](const prefix_and_value_string &pattern) -> void {
    output += pattern.prefix + pattern.value_string;
  };

  if (no_newline_in_value_string) {
    prefix_and_value_string pattern1a = get_prefix_and_value_string(expr + " => ", indent9);

    if (!pattern1a.value_string_has_newline && !pattern1a.over_max_line_width) {
      append_output(pattern1a);
      return true;
    }

    if (_last_line_length(output) <= 9) {
      prefix_and_value_string pattern1b =
          get_prefix_and_value_string(expr + "\n" + indent11 + "=> ", indent11);

      if (!pattern1b.value_string_has_newline) {
        append_output(pattern1b);
        return true;
      }

      return false;
    }

    prefix_and_value_string pattern2a =
        get_prefix_and_value_string("\n" + indent9 + expr + " => ", indent9);

    if (!pattern2a.value_string_has_newline && !pattern2a.over_max_line_width) {
      append_output(pattern2a);
      return true;
    }

    prefix_and_value_string pattern2b =
        get_prefix_and_value_string("\n" + indent9 + expr + "\n" + indent11 + "=> ", indent11);

    if (!pattern2b.value_string_has_newline) {
      append_output(pattern2b);
      return true;
    }

    return false;
  }

  prefix_and_value_string pattern1a = get_prefix_and_value_string(expr + " => ", indent9);

  if (!pattern1a.over_max_line_width) {
    if (!pattern1a.value_string_has_newline) {
      append_output(pattern1a);
      return true;
    }

    prefix_and_value_string pattern1b =
        get_prefix_and_value_string(expr + "\n" + indent11 + "=> ", indent11);

    if (!pattern1b.value_string_has_newline) {
      append_output(pattern1b);
      return true;
    }

    append_output(pattern1a);
    return true;
  }

  prefix_and_value_string pattern1b =
      get_prefix_and_value_string(expr + "\n" + indent11 + "=> ", indent11);

  append_output(pattern1b);
  return true;
}

inline bool _dump_recursive(std::string &, bool) { return true; }

template <typename T, typename... Args>
inline bool _dump_recursive(std::string &output, bool no_newline_in_value_string,
                            const std::string &expr, const T &value, const Args &...args) {
  return _dump_one(output, no_newline_in_value_string, expr, value) &&
         _dump_recursive(output, no_newline_in_value_string, args...);
}

template <typename... Args>
void _dump(const Args &...args) {
  bool no_newline_in_value_string = true;

rollback:
  std::string output = "";
  if (!_dump_recursive(output, no_newline_in_value_string, args...)) {
    no_newline_in_value_string = false;
    goto rollback;
  }

  std::clog << output << std::endl;
}

}  // namespace cpp_dump
