/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

#include "../escape_sequence.hpp"

namespace cpp_dump {

namespace _detail {

inline std::string export_unsupported() { return es::unsupported("Unsupported Type"); }

}  // namespace _detail

}  // namespace cpp_dump
