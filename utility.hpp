/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

namespace cpp_dump {

inline bool _has_newline(const std::string &value) { return value.find("\n") != std::string::npos; }

inline size_t _first_line_length(const std::string &value) {
  auto lf_pos = value.find("\n");

  if (lf_pos == std::string::npos) return value.length();

  return lf_pos;
}

inline size_t _last_line_length(const std::string &value, int additional_first_line_length = 0) {
  auto lf_pos = value.rfind("\n");

  if (lf_pos == std::string::npos) return additional_first_line_length + value.length();

  return value.length() - lf_pos - 1;
}

}  // namespace cpp_dump
