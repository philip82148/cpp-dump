/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>
#include <type_traits>

#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

inline void _replace_string(std::string &subject, std::string_view search,
                            std::string_view replace) {
  std::string::size_type pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

template <typename T>
inline auto export_string(const T &value, const std::string &, size_t, size_t, bool fail_on_newline)
    -> std::enable_if_t<is_string<T>, std::string> {
  std::string str{value};

  _replace_string(str, R"(\)", R"(\\)");

  if (!_has_newline(str) && str.find(R"(")") == std::string::npos) return R"(")" + str + R"(")";

  _replace_string(str, R"(`)", R"(\`)");

  if (!_has_newline(str)) return R"(`)" + str + R"(`)";

  if (fail_on_newline) return "\n";

  return "\n"
         R"(`)" +
         str + R"(`)";
}

}  // namespace cpp_dump
