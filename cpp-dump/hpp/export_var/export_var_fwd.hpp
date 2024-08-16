/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file
 * in the root directory of this source tree.
 */

#pragma once

#include <string>

#include "../export_command/export_command.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

}  // namespace _detail

}  // namespace cpp_dump
