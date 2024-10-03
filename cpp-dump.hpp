/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <string_view>

#include "./cpp-dump/hpp/escape_sequence.hpp"
#include "./cpp-dump/hpp/expand_va_macro.hpp"
#include "./cpp-dump/hpp/export_command/export_command.hpp"
#include "./cpp-dump/hpp/export_var/export_var.hpp"
#include "./cpp-dump/hpp/options.hpp"
#include "./cpp-dump/hpp/utility.hpp"

#define _p_CPP_DUMP_STRINGIFY(x) #x
#define _p_CPP_DUMP_CONTAINS_VARIADIC_TEMPLATE(...)                                                \
  cpp_dump::_detail::contains_variadic_template<_p_CPP_DUMP_VA_SIZE(__VA_ARGS__)>(                 \
      {_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_STRINGIFY, __VA_ARGS__)}                                  \
  )

/**
 * Print string representations of expressions and results to std::clog or other configurable
 * outputs.
 * If you want to change the output, define an explicit specialization of cpp_dump::write_log().
 * This macro uses cpp_dump::export_var() internally.
 */
#define cpp_dump(...)                                                                              \
  cpp_dump::_detail::cpp_dump_macro<                                                               \
      _p_CPP_DUMP_VA_SIZE(__VA_ARGS__),                                                            \
      _p_CPP_DUMP_CONTAINS_VARIADIC_TEMPLATE(__VA_ARGS__)>(                                        \
      {__FILE__, __LINE__, __func__},                                                              \
      {_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_STRINGIFY, __VA_ARGS__)},                                 \
      __VA_ARGS__                                                                                  \
  )

/**
 * This is deprecated.
 * Use cpp_dump() instead.
 */
#define CPP_DUMP(...)                                                                              \
  _Pragma("message(\"WARNING: Deprecated. Use the lowercase 'cpp_dump()' macro instead.\")")       \
      cpp_dump(__VA_ARGS__)

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

    bool over_max_line_width =
        last_line_length + get_first_line_length(value_str) > options::max_line_width;

    return {prefix, value_str, value_str_has_newline, over_max_line_width};
  };

  auto append_output = [&](const prefix_and_value_str &pattern) -> void {
    output += pattern.prefix + pattern.value_str;
  };

  if (!options::print_expr) {
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

template <bool is_va_temp, typename... Args>
bool _dump(
    std::string &output,
    const std::string &log_label,
    bool always_newline_before_expr,
    std::initializer_list<std::string_view> exprs,
    const Args &...args
) {
  if constexpr (is_va_temp) {
    std::string_view first_arg_name = *exprs.begin();
    std::size_t i = 0;
    auto expr = [&]() -> std::string {
      return std::string(first_arg_name) + "[" + std::to_string(i++) + "]";
    };
    return (_dump_one(output, log_label, always_newline_before_expr, expr(), args) && ...);
  } else {
    auto it = exprs.begin();
    return (_dump_one(output, log_label, always_newline_before_expr, *it++, args) && ...);
  }
}

struct _source_location {
  std::string_view file_name;
  std::size_t line;
  std::string_view function_name;
};

// in C++17, std::initializer_list is not a literal type.
template <std::size_t N>
constexpr bool contains_variadic_template(std::array<std::string_view, N> exprs) {
  // std::any_of is not a constexpr function either.
  for (auto expr : exprs) {
    if (expr.size() > 3 && expr.substr(expr.size() - 3) == "...") return true;
  }

  return false;
}

// function called by cpp_dump() macro
template <std::size_t va_macro_size, bool contains_va_temp, typename... Args>
void cpp_dump_macro(
    _source_location loc, std::initializer_list<std::string_view> exprs, const Args &...args
) {
  constexpr bool is_va_temp = va_macro_size == 1 && contains_va_temp;
  static_assert(
      (va_macro_size == sizeof...(args) && !contains_va_temp) || is_va_temp,
      "The number of expressions passed to cpp_dump(...) does not match the number of actual "
      "arguments. Please enclose expressions that contain commas in parentheses. "
      "If you are passing variadic template arguments, do not pass any additional arguments."
  );

  std::string log_label = options::log_label_func
                              ? options::log_label_func(loc.file_name, loc.line, loc.function_name)
                              : "";

  bool exprs_have_newline =
      options::print_expr && std::any_of(exprs.begin(), exprs.end(), has_newline);

  std::string output;
  if (exprs_have_newline || !_dump<is_va_temp>(output, log_label, false, exprs, args...)) {
    output.clear();
    _dump<is_va_temp>(output, log_label, true, exprs, args...);
  }

  write_log(output);
}

}  // namespace _detail

}  // namespace cpp_dump
