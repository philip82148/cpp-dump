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

namespace _detail {

struct empty_class {};

}  // namespace _detail

#define _p_CPP_DUMP_SET_OPTION_IN_GLOBAL_AUX2(variable, value, line)                               \
  namespace cpp_dump {                                                                             \
                                                                                                   \
  namespace _detail {                                                                              \
                                                                                                   \
  namespace _dummy_variables_for_set_option_in_global {                                            \
                                                                                                   \
  [[maybe_unused]] inline auto _dummy_##line = (cpp_dump::variable = (value), empty_class{});      \
                                                                                                   \
  } /* namespace _dummy_variables_for_set_option_in_global */                                      \
                                                                                                   \
  } /* namespace _detail */                                                                        \
                                                                                                   \
  }  // namespace cpp_dump

#define _p_CPP_DUMP_SET_OPTION_IN_GLOBAL_AUX(variable, value, line)                                \
  _p_CPP_DUMP_SET_OPTION_IN_GLOBAL_AUX2(variable, value, line)

/**
 * Set a value to a variable in cpp_dump namespace.
 * You can also assign values to the variables directly.
 */
#define CPP_DUMP_SET_OPTION_IN_GLOBAL(variable, value)                                             \
  _p_CPP_DUMP_SET_OPTION_IN_GLOBAL_AUX(variable, value, __LINE__)

/**
 * Type of cpp_dump::es_style.
 * cpp_dump::export_var() supports this type.
 */
enum class es_style_t { no_es, original, by_syntax };

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
 * Maximum line width of the strings returned by cpp_dump() and cpp_dump::export_var().
 */
inline std::size_t max_line_width = 160;

/**
 * Maximum number of times cpp_dump::export_var() is called recursively.
 */
inline std::size_t max_depth = 4;

/**
 * Maximum number of iterations of cpp_dump::export_var() over an iterator.
 * Note that in a single call, cpp_dump::export_var() calls itself at most
 * (max_iteration_count^(max_depth+1)-1)/(max_iteration_count-1)-1 times.
 */
inline std::size_t max_iteration_count = 16;

/**
 * Whether cpp_dump() prints types of the Asterisk category (See 'Supported types').
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
 * Style of the escape sequences (output coloring).
 */
inline es_style_t es_style = es_style_t::original;

/**
 * Values of the escape sequences (output coloring).
 */
inline es_value_t es_value;

/**
 * Style of indents of the Container, Set and Map categories (See 'Supported types').
 */
inline cont_indent_style_t cont_indent_style = cont_indent_style_t::when_nested;

}  // namespace cpp_dump
