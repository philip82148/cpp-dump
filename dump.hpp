#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "export_var.hpp"
#include "utility.hpp"

#define EXPAND_VA_ARGS1_(arg1) #arg1, (arg1)
#define EXPAND_VA_ARGS2_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS1_(__VA_ARGS__)
#define EXPAND_VA_ARGS3_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS2_(__VA_ARGS__)
#define EXPAND_VA_ARGS4_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS3_(__VA_ARGS__)
#define EXPAND_VA_ARGS5_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS4_(__VA_ARGS__)
#define EXPAND_VA_ARGS6_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS5_(__VA_ARGS__)
#define EXPAND_VA_ARGS7_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS6_(__VA_ARGS__)
#define EXPAND_VA_ARGS8_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS7_(__VA_ARGS__)
#define EXPAND_VA_ARGS9_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS8_(__VA_ARGS__)
#define EXPAND_VA_ARGS10_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS9_(__VA_ARGS__)
#define EXPAND_VA_ARGS11_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS10_(__VA_ARGS__)
#define EXPAND_VA_ARGS12_(arg1, ...) EXPAND_VA_ARGS1_(arg1), EXPAND_VA_ARGS11_(__VA_ARGS__)

#define VA_ARGS_SIZE_AUX_(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, \
                          arg12, size, ...)                                                   \
  size
#define VA_ARGS_SIZE_(...) VA_ARGS_SIZE_AUX_(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define GET_EXPAND_VA_ARGS_MACRO_(size) EXPAND_VA_ARGS##size##_
#define EXPAND_VA_ARGS_(size, ...) GET_EXPAND_VA_ARGS_MACRO_(size)(__VA_ARGS__)

#define dump(...) cpp_dump::_dump(EXPAND_VA_ARGS_(VA_ARGS_SIZE_(__VA_ARGS__), __VA_ARGS__))

namespace cpp_dump {

inline const int max_line_width = 80;

template <typename T>
bool _dump_one(std::string &output, bool value_string_in_one_line, std::string expr, T &&value) {
  const std::string indent9 = "         ";
  const std::string indent11 = "           ";

  if (output.length() == 0) {
    output = "[dump()] ";
  } else {
    if (value_string_in_one_line) {
      output += ", ";
    } else {
      output += ",\n" + indent9;
    }
  }

  struct prefix_and_value_string {
    std::string prefix;
    std::string value_string;
    bool over_max_line_width;
    bool value_string_has_lf;
  };

  auto get_prefix_and_value_string = [&](std::string prefix,
                                         std::string indent) -> prefix_and_value_string {
    auto last_line_length = _last_line_length(output + prefix);

    std::string value_string = export_var(value, indent, last_line_length);

    bool over_max_line_width = last_line_length + _first_line_length(value_string) > max_line_width;

    bool value_string_has_lf = _has_lf(value_string);

    return {prefix, value_string, over_max_line_width, value_string_has_lf};
  };

  auto append_output = [&](prefix_and_value_string &pattern) -> void {
    output += pattern.prefix + pattern.value_string;
  };

  if (!value_string_in_one_line) {
    prefix_and_value_string pattern1a = get_prefix_and_value_string(expr + " => ", indent9);

    if (!pattern1a.over_max_line_width) {
      if (!pattern1a.value_string_has_lf) {
        append_output(pattern1a);
        return true;
      }

      prefix_and_value_string pattern1b =
          get_prefix_and_value_string(expr + "\n" + indent11 + "=> ", indent11);

      if (!pattern1b.value_string_has_lf) {
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

  prefix_and_value_string pattern1a = get_prefix_and_value_string(expr + " => ", indent9);

  if (!pattern1a.over_max_line_width && !pattern1a.value_string_has_lf) {
    append_output(pattern1a);
    return true;
  }

  if (_last_line_length(output) <= 9) {
    prefix_and_value_string pattern1b =
        get_prefix_and_value_string(expr + "\n" + indent11 + "=> ", indent11);

    if (!pattern1b.value_string_has_lf) {
      append_output(pattern1b);
      return true;
    }

    return false;
  }

  prefix_and_value_string pattern2a =
      get_prefix_and_value_string("\n" + indent9 + expr + " => ", indent9);

  if (!pattern2a.over_max_line_width && !pattern2a.value_string_has_lf) {
    append_output(pattern2a);
    return true;
  }

  prefix_and_value_string pattern2b =
      get_prefix_and_value_string("\n" + indent9 + expr + "\n" + indent11 + "=> ", indent11);

  if (!pattern2b.value_string_has_lf) {
    append_output(pattern2b);
    return true;
  }

  return false;
}

inline bool _dump_recursive(std::string &, bool) { return true; }

template <typename T, typename... Args>
inline bool _dump_recursive(std::string &output, bool value_string_in_one_line, std::string expr,
                            T &&value, Args &&...args) {
  return _dump_one(output, value_string_in_one_line, expr, value) &&
         _dump_recursive(output, value_string_in_one_line, args...);
}

template <typename... Args>
void _dump(Args &&...args) {
  bool value_string_in_one_line = true;

rollback:
  std::string output = "";
  if (!_dump_recursive(output, value_string_in_one_line, args...)) {
    value_string_in_one_line = false;
    goto rollback;
  }

  std::clog << output << std::endl;
}

}  // namespace cpp_dump
