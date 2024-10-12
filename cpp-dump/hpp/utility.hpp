/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <string_view>

#include "./escape_sequence.hpp"

namespace cpp_dump {

namespace _detail {

inline bool has_newline(std::string_view s) { return s.find('\n') != std::string::npos; }

inline std::size_t get_length(std::string_view s) {
  if (!use_es()) return s.length();

  static constexpr std::string_view es_begin_token = "\x1b[";

  std::size_t length = 0;
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::search(begin, s.end(), es_begin_token.begin(), es_begin_token.end()))
         != s.end()) {
    length += end - begin;

    begin = end + es_begin_token.size();
    end = std::find_if(begin, s.end(), [](char c) {
      return !(std::isdigit(static_cast<unsigned char>(c)) || c == ';');
    });

    if (end == s.end()) break;
    begin = end + 1;
  }
  length += end - begin;

  return length;
}

inline std::size_t get_first_line_length(std::string_view s) {
  auto lf_pos = s.find('\n');

  if (lf_pos == std::string::npos) return get_length(s);

  return get_length(s.substr(0, lf_pos));
}

inline std::size_t get_last_line_length(
    std::string_view s, std::size_t additional_first_line_length = 0
) {
  auto lf_pos = s.rfind('\n');

  if (lf_pos == std::string::npos) return additional_first_line_length + get_length(s);

  return get_length(s.substr(lf_pos + 1));
}

inline std::string replace_string(
    std::string_view s, std::string_view search, std::string_view replace
) {
  std::string retval;

  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::search(begin, s.end(), search.begin(), search.end())) != s.end()) {
    retval.append(begin, end);
    retval.append(replace);
    begin = end + search.size();
  }
  retval.append(begin, s.end());

  return retval;
}

inline std::string escape_non_printable_char(char c) {
  static const std::map<char, std::string_view> char_to_escaped{
      {'\0', "\\0"},  // null
      {'\a', "\\a"},  // bell
      {'\b', "\\b"},  // backspace
      {'\f', "\\f"},  // form feed
      {'\n', "\\n"},  // LF
      {'\r', "\\r"},  // CR
      {'\t', "\\t"},  // Horizontal tab
      {'\v', "\\v"},  // Vertical tab
  };

  if (char_to_escaped.count(c)) return std::string(char_to_escaped.at(c));

  auto to_hex_char = [](unsigned char uc) -> char {
    return static_cast<char>(uc < 10 ? '0' + uc : 'A' + (uc - 10));
  };
  char upper = to_hex_char((c >> 4) & 0x0f);
  char lower = to_hex_char(c & 0x0f);

  return std::string({'\\', 'x', upper, lower});
}

inline std::string escape_string(std::string_view s) {
  auto need_escape = [](char c) {
    return !std::isprint(static_cast<unsigned char>(c)) || c == '"' || c == '\\';
  };
  auto escape = [](char c) -> std::string {
    if (c == '"') return R"(\")";
    if (c == '\\') return R"(\\)";
    return escape_non_printable_char(c);
  };

  std::string retval(1, '"');
  auto begin = s.begin();
  decltype(begin) end;
  while ((end = std::find_if(begin, s.end(), need_escape)) != s.end()) {
    retval.append(begin, end);
    retval.append(escape(*end));
    begin = end + 1;
  }
  retval.append(begin, end).push_back('"');

  return retval;
}

}  // namespace _detail

}  // namespace cpp_dump
