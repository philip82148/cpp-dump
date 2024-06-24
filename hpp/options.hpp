/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include "./log_label.hpp"

namespace cpp_dump {

/**
 * Set a value to a variable in cpp_dump namespace.
 * You can also assign values to the variables directly.
 */
#define CPP_DUMP_SET_OPTION(variable, value) cpp_dump::variable = (value)

/**
 * You can execute a function before the main by defining a global variable of this class.
 * If you define the perform static member in this class instead of a global variable,
 * it won't pollute the namespace.
 * Pay attention to the static initialization order fiasco
 * ( https://isocpp.org/wiki/faq/ctors#static-init-order ).
 */
struct execute_before_main {
  template <typename Func>
  execute_before_main(Func func) {
    func();
  }
  static execute_before_main perform;
};

/**
 * Type of cpp_dump::es_style.
 * cpp_dump::export_var() supports this type.
 */
enum class es_style_t { no_es, by_syntax, by_syntax2_experimental };

/**
 * Type of cpp_dump::es_value.
 * cpp_dump::export_var() supports this type.
 */
struct es_value_t {
  std::string log = "\x1b[02m";                           // dark
  std::string expression = "\x1b[36m";                    // cyan
  std::string reserved;                                   // default
  std::string number;                                     // default
  std::string character;                                  // default
  std::string op = "\x1b[02m";                            // dark
  std::string identifier = "\x1b[32m";                    // green
  std::string member = "\x1b[36m";                        // cyan
  std::string unsupported = "\x1b[31m";                   // red
  std::vector<std::string> bracket_by_depth{"\x1b[02m"};  // dark
};

/**
 * Type of cpp_dump::cont_indent_style.
 * cpp_dump::export_var() supports this type.
 */
enum class cont_indent_style_t { minimal, when_nested, when_non_tuples_nested, always };

/**
 * Maximum line width of output strings of cpp_dump::export_var().
 */
inline std::size_t max_line_width = 160;

/**
 * Maximum number of times cpp_dump::export_var() is applied recursively.
 */
inline std::size_t max_depth = 4;

/**
 * Maximum number of times cpp_dump::export_var() iterates over an iterator.
 * Note that in a single call, export_var() calls itself at most
 * (max_iteration_count^(max_depth+1)-1)/(max_iteration_count-1)-1 times.
 */
inline std::size_t max_iteration_count = 16;

/**
 * Whether cpp_dump() prints types of the Asterisk category (See README.md).
 */
inline bool enable_asterisk = false;

/**
 * Whether cpp_dump() prints the expressions.
 */
inline bool print_expr = true;

/**
 * Function that returns the label that cpp_dump() prints at the beginning of the output.
 */
inline log_label::log_label_func_t log_label_func = log_label::default_func;

/**
 * Style of the escape sequences.
 */
inline es_style_t es_style = es_style_t::by_syntax;

/**
 * Value of the escape sequences.
 */
inline es_value_t es_value;

/**
 * Style of indents of containers.
 */
inline cont_indent_style_t cont_indent_style = cont_indent_style_t::when_nested;

}  // namespace cpp_dump
