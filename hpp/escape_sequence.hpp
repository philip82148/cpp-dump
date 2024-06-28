/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include "./options.hpp"

namespace cpp_dump {

namespace _detail {

inline bool use_es() { return es_style != es_style_t::no_es; }

namespace es {

inline std::string apply(const std::string &es, const std::string &s) {
  if (use_es()) {
    static constexpr char reset[] = "\x1b[0m";
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

inline std::string type_name(const std::string &s, bool is_enumerator = false) {
  if (!use_es()) return s;

  auto is_operator = [](char c) { return !(std::isalnum(c) || c == '_'); };

  std::string typename_with_es;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    typename_with_es += es::identifier(std::string(begin, end));
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    typename_with_es += es::op(std::string(begin, end));

    if (end == s.end()) return typename_with_es;
    begin = end;
  }

  if (is_enumerator) {
    typename_with_es += es::member(std::string(begin, s.end()));
  } else {
    typename_with_es += es::identifier(std::string(begin, s.end()));
  }

  return typename_with_es;
}

}  // namespace es

}  // namespace _detail

}  // namespace cpp_dump
