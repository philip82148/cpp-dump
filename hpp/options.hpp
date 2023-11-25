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
#define CPP_DUMP_SET_OPTION(variable, value) cpp_dump::variable = value

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
  std::string reserved;
  std::string number;
  std::string character;
  std::string op;
  std::string identifier;
  std::string member;
  std::string unsupported;
  std::vector<std::string> bracket_by_depth;
  es_value_t(
      std::string log_,
      std::string expression_,
      std::string reserved_,
      std::string number_,
      std::string character_,
      std::string op_,
      std::string identifier_,
      std::string member_,
      std::string unsupported_,
      std::vector<std::string> bracket_by_depth_
  )
      : log(log_),
        expression(expression_),
        reserved(reserved_),
        number(number_),
        character(character_),
        op(op_),
        identifier(identifier_),
        member(member_),
        unsupported(unsupported_),
        bracket_by_depth(bracket_by_depth_) {}
};

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
inline es_value_t es_value = {
    "\x1b[02m",    // log: dark
    "\x1b[36m",    // expression: cyan
    "",            // reserved: default
    "",            // number: default
    "",            // character: default
    "\x1b[02m",    // op: dark
    "\x1b[32m",    // identifier: green
    "\x1b[36m",    // member: cyan
    "\x1b[31m",    // unsupported: red
    {"\x1b[02m"},  // bracket_by_depth[0]: dark
};

}  // namespace cpp_dump
