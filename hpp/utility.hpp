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

inline bool _has_newline(std::string_view s) { return s.find("\n") != std::string::npos; }

inline size_t _first_line_length(std::string_view s) {
  auto lf_pos = s.find("\n");

  if (lf_pos == std::string::npos) return s.length();

  return lf_pos;
}

inline size_t _last_line_length(std::string_view s, int additional_first_line_length = 0) {
  auto lf_pos = s.rfind("\n");

  if (lf_pos == std::string::npos) return additional_first_line_length + s.length();

  return s.length() - lf_pos - 1;
}

}  // namespace cpp_dump
