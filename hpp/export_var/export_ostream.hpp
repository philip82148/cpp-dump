/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <sstream>
#include <string>
#include <type_traits>

#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "./export_unsupported.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
inline auto
export_ostream(const T &value, const std::string &, std::size_t, std::size_t, bool, const export_command &)
    -> std::enable_if_t<is_ostream<T>, std::string> {
  std::ostringstream ss;
  ss << value;

  std::string output = ss.str();
  return output == "" ? export_unsupported() : output;
}

}  // namespace _detail

}  // namespace cpp_dump
