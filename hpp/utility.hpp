/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>

namespace cpp_dump {

namespace _detail {

inline bool use_es();

inline bool has_newline(std::string_view s) { return s.find("\n") != std::string::npos; }

inline size_t get_length(std::string_view s) {
  if (!use_es()) return s.length();

  size_t length = 0, s_first = 0, s_last;
  while ((s_last = s.find("\e[", s_first)) != std::string::npos) {
    length       += s_last - s_first;
    auto es_last = s.find_first_not_of("0123456789;", s_last + 2);
    if (es_last == std::string::npos) break;
    s_first = es_last + 1;
  }
  length += s.length() - s_first;

  return length;
}

inline size_t get_first_line_length(std::string_view s) {
  auto lf_pos = s.find("\n");

  if (lf_pos == std::string::npos) return get_length(s);

  return get_length(s.substr(0, lf_pos));
}

inline size_t get_last_line_length(std::string_view s, int additional_first_line_length = 0) {
  auto lf_pos = s.rfind("\n");

  if (lf_pos == std::string::npos) return additional_first_line_length + get_length(s);

  return get_length(s.substr(lf_pos + 1));
}

inline void replace_string(
    std::string &subject, std::string_view search, std::string_view replace
) {
  std::string::size_type pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

}  // namespace _detail

}  // namespace cpp_dump
