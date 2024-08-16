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

inline bool use_es() { return options::es_style != types::es_style_t::no_es; }

namespace es {

inline constexpr std::string_view _reset_es = "\x1b[0m";

inline std::string reset() { return use_es() ? std::string(_reset_es) : std::string(); }

inline std::string apply(std::string_view es, std::string_view s) {
  if (use_es()) {
    std::string retval(es);
    retval.append(s).append(_reset_es);
    return retval;
  } else {
    return std::string(s);
  }
}

inline std::string log(std::string_view s) { return es::apply(options::es_value.log, s); }
inline std::string expression(std::string_view s) {
  return es::apply(options::es_value.expression, s);
}
inline std::string reserved(std::string_view s) { return es::apply(options::es_value.reserved, s); }
inline std::string number(std::string_view s) { return es::apply(options::es_value.number, s); }
inline std::string character(std::string_view s) {
  return es::apply(options::es_value.character, s);
}
inline std::string op(std::string_view s) { return es::apply(options::es_value.op, s); }
inline std::string identifier(std::string_view s) {
  return es::apply(options::es_value.identifier, s);
}
inline std::string member(std::string_view s) { return es::apply(options::es_value.member, s); }
inline std::string unsupported(std::string_view s) {
  return es::apply(options::es_value.unsupported, s);
}
inline std::string class_op(std::string_view s) { return es::apply(options::es_value.class_op, s); }
inline std::string member_op(std::string_view s) {
  return es::apply(options::es_value.member_op, s);
}
inline std::string number_op(std::string_view s) {
  return es::apply(options::es_value.number_op, s);
}
inline std::string escaped_char(std::string_view s) {
  return es::apply(options::es_value.escaped_char, s);
}

inline std::string bracket(std::string_view s, std::size_t d) {
  auto sz = options::es_value.bracket_by_depth.size();
  if (sz == 0) return std::string(s);

  return es::apply(options::es_value.bracket_by_depth[d % sz], s);
}

inline std::string type_name(std::string_view s) {
  if (!use_es()) return std::string(s);

  auto is_operator = [](char c) {
    return !(std::isalnum(static_cast<unsigned char>(c)) || c == '_');
  };

  std::string output;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    output += es::identifier({&*begin, static_cast<std::size_t>(end - begin)});
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    output += es::class_op({&*begin, static_cast<std::size_t>(end - begin)});

    if (end == s.end()) return output;
    begin = end;
  }
  output += es::identifier({&*begin, static_cast<std::size_t>(s.end() - begin)});

  return output;
}

inline std::string class_name(std::string_view s) {
  if (!use_es()) return std::string(s);
  if (!options::detailed_class_es) return es::identifier(s);

  return es::type_name(s);
}

inline std::string enumerator(std::string_view s) {
  if (!use_es()) return std::string(s);

  auto is_operator = [](char c) {
    return !(std::isalnum(static_cast<unsigned char>(c)) || c == '_');
  };

  auto op_rbegin = std::find_if(s.rbegin(), s.rend(), is_operator);
  if (op_rbegin == s.rend()) return es::member(s);
  auto op_end = op_rbegin.base();

  auto op_rend = std::find_if_not(op_rbegin, s.rend(), is_operator);
  if (op_rend == s.rend()) {
    return es::op({&*s.begin(), static_cast<std::size_t>(op_end - s.begin())})
           + es::member({&*op_end, static_cast<std::size_t>(s.end() - op_end)});
  }
  auto op_begin = op_rend.base();

  return es::class_name({&*s.begin(), static_cast<std::size_t>(op_begin - s.begin())})
         + es::op({&*op_begin, static_cast<std::size_t>(op_end - op_begin)})
         + es::member({&*op_end, static_cast<std::size_t>(s.end() - op_end)});
}

inline std::string class_member(std::string_view s) {
  if (!use_es()) return std::string(s);
  if (!options::detailed_member_es) return es::member(s);

  auto is_operator = [](char c) {
    return !(std::isalnum(static_cast<unsigned char>(c)) || c == '_');
  };

  std::string output;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    output += es::member({&*begin, static_cast<std::size_t>(end - begin)});
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    output += es::member_op({&*begin, static_cast<std::size_t>(end - begin)});

    if (end == s.end()) return output;
    begin = end;
  }
  output += es::member({&*begin, static_cast<std::size_t>(s.end() - begin)});

  return output;
}

inline std::string signed_number(std::string_view s) {
  if (!use_es()) return std::string(s);
  if (!options::detailed_number_es) return es::number(s);

  auto is_operator = [](char c) {
    return !(std::isalnum(static_cast<unsigned char>(c)) || c == '.');
  };

  std::string output;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_operator)) != s.end()) {
    if (begin != end) output += es::number({&*begin, static_cast<std::size_t>(end - begin)});
    begin = end;

    end = std::find_if_not(begin, s.end(), is_operator);
    output += es::number_op({&*begin, static_cast<std::size_t>(end - begin)});

    if (end == s.end()) return output;
    begin = end;
  }
  output += es::number({&*begin, static_cast<std::size_t>(s.end() - begin)});

  return output;
}

inline std::string escaped_str(std::string_view s) {
  if (!use_es()) return std::string(s);

  auto is_backslash = [](char c) { return c == '\\'; };

  std::string output;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), is_backslash)) != s.end()) {
    if (begin != end) output += es::character({&*begin, static_cast<std::size_t>(end - begin)});
    begin = end;

    while (*end == '\\') {
      if (++end == s.end()) break;
      if (*end == 'x') {
        if (++end == s.end()) break;
        if (++end == s.end()) break;
        if (++end == s.end()) break;
      } else {
        if (++end == s.end()) break;
      }
    }

    output += es::escaped_char({&*begin, static_cast<std::size_t>(end - begin)});

    if (end == s.end()) return output;
    begin = end;
  }
  output += es::character({&*begin, static_cast<std::size_t>(s.end() - begin)});

  return output;
}

}  // namespace es

}  // namespace _detail

}  // namespace cpp_dump
