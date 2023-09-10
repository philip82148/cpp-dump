/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <string>
#include <type_traits>

#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename... Args>
inline std::string export_other(const std::reference_wrapper<Args...> &ref,
                                const std::string &indent, size_t last_line_length,
                                size_t current_depth, bool fail_on_newline) {
  return export_var(ref.get(), indent, last_line_length, current_depth, fail_on_newline);
}

template <size_t N>
inline std::string export_other(const std::bitset<N> &bitset, const std::string &, size_t, size_t,
                                bool) {
  std::string bitset_str = bitset.to_string();

  std::string output;
  for (int end = bitset_str.length(); end > 0; end -= 4) {
    size_t begin = std::max(end - 4, 0);
    if (output == "") {
      output = bitset_str.substr(begin, end - begin);
    } else {
      output = bitset_str.substr(begin, end - begin) + " " + output;
    }
  }
  output = "0b " + output;

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
