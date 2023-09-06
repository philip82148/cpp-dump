/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <bitset>
#include <string>
#include <type_traits>

#include "./type_check.hpp"

namespace cpp_dump {

extern inline size_t max_line_width;

extern inline size_t max_depth;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
inline auto export_bitset(const T &bitset, const std::string &, size_t, size_t, bool)
    -> std::enable_if_t<is_bitset<T>, std::string> {
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
