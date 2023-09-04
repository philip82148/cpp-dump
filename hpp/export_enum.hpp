/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <map>
#include <string>

#include "./expand_va_macro.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

#define CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_(member) \
  { member, #member }
#define CPP_DUMP_DEFINE_EXPORT_ENUM(type, ...)                                                \
  namespace cpp_dump {                                                                        \
                                                                                              \
  extern inline size_t max_line_width;                                                        \
                                                                                              \
  extern inline size_t max_depth;                                                             \
                                                                                              \
  namespace _detail {                                                                         \
                                                                                              \
  template <>                                                                                 \
  inline constexpr bool _is_exportable_enum<type> = true;                                     \
                                                                                              \
  template <typename T>                                                                       \
  std::string export_var(const T &, const std::string &, size_t, size_t, bool);               \
                                                                                              \
  template <>                                                                                 \
  inline std::string export_enum(const type &enum_const, const std::string &, size_t, size_t, \
                                 bool) {                                                      \
    std::map<type, std::string> enum_to_string{                                               \
        CPP_DUMP_EXPAND_VA_(CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_, __VA_ARGS__)};                  \
    return enum_to_string.count(enum_const) ? enum_to_string[enum_const] : #type "::?";       \
  }                                                                                           \
                                                                                              \
  } /* namespace _detail */                                                                   \
                                                                                              \
  }  // namespace cpp_dump

namespace cpp_dump {

namespace _detail {

template <typename T>
inline std::string export_enum(const T &, const std::string &, size_t, size_t, bool);

}  // namespace _detail

}  // namespace cpp_dump
