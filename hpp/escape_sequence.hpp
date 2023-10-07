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

struct escape_sequence {
 public:
  std::string reserved;
  std::string number;
  std::string character;
  std::string identifier;
  std::string member;
  std::string unsupported;
  std::string expression;
  std::vector<std::string> bracket_by_depth;
};

enum class use_es { do_not_use, normal, enhanced };

namespace es_theme {

const escape_sequence simple = {
    "\e[34m",
    "\e[32m",
    "\e[36m",
    "\e[34m",
    "\e[34m",
};

const escape_sequence rich = {
    "\e[38;2;53;140;214m",
    "\e[38;2;167;206;155m",
    "\e[38;2;195;145;91m",
    "\e[38;2;78;201;176m",
    "\e[38;2;103;205;254m",
    "",
    "\e[38;2;23;159;255m",
    // "\e[38;2;68;193;241m",
    {"\e[38;2;255;215;0m", "\e[38;2;218;112;214m", "\e[38;2;23;159;255m"},
};

};  // namespace es_theme

extern inline bool use_es;

extern inline escape_sequence es_value;

namespace _detail {

namespace with_es {

inline std::string _apply(const std::string &es, const std::string &s) {
  if (use_es) {
    const std::string reset = "\e[0m";
    return es + s + reset;
  } else {
    return s;
  }
}

inline std::string reserved(const std::string &s) { return _apply(es_value.reserved, s); }
inline std::string number(const std::string &s) { return _apply(es_value.number, s); }
inline std::string character(const std::string &s) { return _apply(es_value.character, s); }
inline std::string identifier(const std::string &s) { return _apply(es_value.identifier, s); }
inline std::string member(const std::string &s) { return _apply(es_value.member, s); }
inline std::string unsupported(const std::string &s) { return _apply(es_value.unsupported, s); }
inline std::string expression(const std::string &s) { return _apply(es_value.expression, s); }

inline std::string bracket(const std::string &s, size_t d) {
  if (es_value.bracket_by_depth.size() == 0) return s;

  return _apply(es_value.bracket_by_depth[d % es_value.bracket_by_depth.size()], s);
}

}  // namespace with_es

}  // namespace _detail

}  // namespace cpp_dump