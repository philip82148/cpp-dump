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
#include <string_view>
#include <vector>

#include "./options.hpp"

namespace cpp_dump {

namespace _detail {

inline bool use_es() { return es_style != es_style_t::no_es; }

namespace es {

inline constexpr std::string_view _reset_es = "\x1b[0m";

inline std::string reset() { return use_es() ? std::string(_reset_es) : std::string(); }

inline std::string apply(std::string_view es, std::string_view s) {
  if (use_es()) {
    return std::string(es).append(s).append(_reset_es);
  } else {
    return std::string(s);
  }
}

inline std::string log(std::string_view s) { return es::apply(es_value.log, s); }
inline std::string expression(std::string_view s) { return es::apply(es_value.expression, s); }
inline std::string reserved(std::string_view s) { return es::apply(es_value.reserved, s); }
inline std::string number(std::string_view s) { return es::apply(es_value.number, s); }
inline std::string character(std::string_view s) { return es::apply(es_value.character, s); }
inline std::string op(std::string_view s) { return es::apply(es_value.op, s); }
inline std::string identifier(std::string_view s) { return es::apply(es_value.identifier, s); }
inline std::string member(std::string_view s) { return es::apply(es_value.member, s); }
inline std::string unsupported(std::string_view s) { return es::apply(es_value.unsupported, s); }

inline std::string bracket(std::string_view s, std::size_t d) {
  auto sz = es_value.bracket_by_depth.size();
  if (sz == 0) return std::string(s);

  return es::apply(es_value.bracket_by_depth[d % sz], s);
}

inline std::string class_name(std::string_view s, bool is_enumerator = false) {
  if (!use_es()) return std::string(s);

  auto is_operator = [](char c) { return !(std::isalnum(c) || c == '_'); };

  std::string typename_with_es;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    typename_with_es += es::identifier(std::string_view(&*begin, end - begin));
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    typename_with_es += es::op(std::string_view(&*begin, end - begin));

    if (end == s.end()) return typename_with_es;
    begin = end;
  }

  if (is_enumerator) {
    typename_with_es += es::member(std::string_view(&*begin, s.end() - begin));
  } else {
    typename_with_es += es::identifier(std::string_view(&*begin, s.end() - begin));
  }

  return typename_with_es;
}

inline std::string class_member(std::string_view s) {
  if (!use_es()) return std::string(s);

  auto is_operator = [](char c) { return !(std::isalnum(c) || c == '_'); };

  std::string typename_with_es;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    typename_with_es += es::member(std::string_view(&*begin, end - begin));
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    typename_with_es += es::op(std::string_view(&*begin, end - begin));

    if (end == s.end()) return typename_with_es;
    begin = end;
  }
  typename_with_es += es::member(std::string_view(&*begin, s.end() - begin));

  return typename_with_es;
}

}  // namespace es

}  // namespace _detail

}  // namespace cpp_dump
