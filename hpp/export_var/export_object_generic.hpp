/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>

#include "../escape_sequence.hpp"
#include "../expand_va_macro.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "./export_object_common.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_GENERIC(member)  value.member
#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_GENERIC2(member) append_output(#member, value.member)

/**
 * Make cpp_dump::export_var() support every type that has the specified members.
 * Member functions to be displayed must be const.
 * Compile errors in this macro, such as ambiguous function calls, are never reported due to SFINAE.
 */
#define CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(...)                                                                 \
  namespace cpp_dump {                                                                                             \
                                                                                                                   \
  namespace _detail {                                                                                              \
                                                                                                                   \
  template <typename T>                                                                                            \
  inline auto export_object_generic(                                                                               \
      const T &value,                                                                                              \
      const std::string &indent,                                                                                   \
      std::size_t last_line_length,                                                                                \
      std::size_t current_depth,                                                                                   \
      bool fail_on_newline,                                                                                        \
      const export_command &command                                                                                \
  ) -> decltype(_p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_GENERIC, __VA_ARGS__), std::string()) { \
    std::string class_name = es::class_name(get_typename<T>());                                                    \
                                                                                                                   \
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;                                                                      \
                                                                                                                   \
    _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT_GENERIC2, __VA_ARGS__);                             \
                                                                                                                   \
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;                                                                      \
  }                                                                                                                \
                                                                                                                   \
  } /* namespace _detail */                                                                                        \
                                                                                                                   \
  }  // namespace cpp_dump

/**
 * This is deprecated.
 * Use CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC() instead.
 */
#define CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(...)                                               \
  _Pragma(                                                                                         \
      "message (\"WARNING: Deprecated. Use CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC() instead.\")"    \
  ) CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(__VA_ARGS__)
