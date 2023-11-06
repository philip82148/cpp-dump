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

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

template <typename T>
inline auto export_ostream(
    const T &value,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_ostream<T>, std::string> {
  std::ostringstream ss;
  ss << value;

  std::string output = ss.str();
  if (output != "") return output;

  struct unsupported {};
  return export_var(
      unsupported(), indent, last_line_length, current_depth, fail_on_newline, command
  );
}

}  // namespace _detail

}  // namespace cpp_dump
