/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <vector>

namespace cpp_dump {

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
      std::string log,
      std::string expression,
      std::string reserved,
      std::string number,
      std::string character,
      std::string op,
      std::string identifier,
      std::string member,
      std::string unsupported,
      std::vector<std::string> bracket_by_depth
  )
      : log(log),
        expression(expression),
        reserved(reserved),
        number(number),
        character(character),
        op(op),
        identifier(identifier),
        member(member),
        unsupported(unsupported),
        bracket_by_depth(bracket_by_depth) {}
};

extern inline es_style_t es_style;

extern inline es_value_t es_value;

namespace _detail {

inline bool use_es() { return es_style != es_style_t::no_es; }

namespace es {

inline std::string apply(const std::string &es, const std::string &s) {
  if (use_es()) {
    const std::string reset = "\x1b[0m";
    return reset + es + s + reset;
  } else {
    return s;
  }
}

inline std::string log(const std::string &s) { return es::apply(es_value.log, s); }
inline std::string expression(const std::string &s) { return es::apply(es_value.expression, s); }
inline std::string reserved(const std::string &s) { return es::apply(es_value.reserved, s); }
inline std::string number(const std::string &s) { return es::apply(es_value.number, s); }
inline std::string character(const std::string &s) { return es::apply(es_value.character, s); }
inline std::string op(const std::string &s) { return es::apply(es_value.op, s); }
inline std::string identifier(const std::string &s) { return es::apply(es_value.identifier, s); }
inline std::string member(const std::string &s) { return es::apply(es_value.member, s); }
inline std::string unsupported(const std::string &s) { return es::apply(es_value.unsupported, s); }

inline std::string bracket(const std::string &s, std::size_t d) {
  auto sz = es_value.bracket_by_depth.size();
  if (sz == 0) return s;

  return es::apply(es_value.bracket_by_depth[d % sz], s);
}

}  // namespace es

}  // namespace _detail

}  // namespace cpp_dump
