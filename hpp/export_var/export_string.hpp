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

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto
export_string(const T &value, const std::string &, std::size_t, std::size_t, bool fail_on_newline, const export_command &)
    -> std::enable_if_t<is_string<T>, std::string> {
  std::string str{value};

  // replace_string(str, R"(\)", R"(\\)");

  if (!has_newline(str) && str.find(R"(")") == std::string::npos)
    return es::character(R"(")" + str) + es::character(R"(")");

  // replace_string(str, R"(`)", R"(\`)");

  if (!has_newline(str)) return es::character(R"(`)" + str) + es::character(R"(`)");

  if (fail_on_newline) return "\n";

  return "\n" + es::character(R"(`)" + str) + es::character(R"(`)");
}

}  // namespace _detail

}  // namespace cpp_dump
