/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <map>
#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../expand_va_macro.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_GENERIC(member) T::member
#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_GENERIC2(member)                                        \
  { T::member, #member }

/**
 * Make cpp_dump::export_var() support every enum type that has the specified members.
 * Compile errors in this macro, such as ambiguous function calls, are never reported due to SFINAE.
 */
#define CPP_DUMP_DEFINE_EXPORT_ENUM_GENERIC(...)                                                                     \
  namespace cpp_dump {                                                                                               \
                                                                                                                     \
  namespace _detail {                                                                                                \
                                                                                                                     \
  template <typename T>                                                                                              \
  inline auto                                                                                                        \
  export_enum_generic(T value, const std::string &, std::size_t, std::size_t, bool, const export_command &)          \
      -> std::enable_if_t<                                                                                           \
          std::is_enum_v<T>,                                                                                         \
          decltype(_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_GENERIC, __VA_ARGS__), std::string())> { \
    static const std::map<T, std::string_view> enum_to_string{                                                       \
        _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM_GENERIC2, __VA_ARGS__)};                            \
    return es::class_name(get_typename<T>()) + es::op("::")                                                          \
           + (enum_to_string.count(value) ? es::member(enum_to_string.at(value))                                     \
                                          : es::unsupported("?"));                                                   \
  }                                                                                                                  \
                                                                                                                     \
  } /* namespace _detail */                                                                                          \
                                                                                                                     \
  }  // namespace cpp_dump
