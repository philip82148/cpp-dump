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

#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT(member) append_output(#member, value.member)

/**
 * Make cpp_dump::export_var() support type TYPE.
 * Member functions to be displayed must be const.
 */
#define CPP_DUMP_DEFINE_EXPORT_OBJECT(TYPE, ...)                                                   \
  namespace cpp_dump {                                                                             \
                                                                                                   \
  namespace _detail {                                                                              \
                                                                                                   \
  template <>                                                                                      \
  inline constexpr bool _is_exportable_object<TYPE> = true;                                        \
                                                                                                   \
  template <>                                                                                      \
  inline std::string export_object(                                                                \
      const TYPE &value,                                                                           \
      const std::string &indent,                                                                   \
      std::size_t last_line_length,                                                                \
      std::size_t current_depth,                                                                   \
      bool fail_on_newline,                                                                        \
      const export_command &command                                                                \
  ) {                                                                                              \
    std::string class_name = es::class_name(#TYPE);                                                \
                                                                                                   \
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON1;                                                      \
                                                                                                   \
    _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_OBJECT, __VA_ARGS__);                      \
                                                                                                   \
    _p_CPP_DUMP_DEFINE_EXPORT_OBJECT_COMMON2;                                                      \
  }                                                                                                \
                                                                                                   \
  } /* namespace _detail */                                                                        \
                                                                                                   \
  }  // namespace cpp_dump

namespace cpp_dump {

namespace _detail {

template <typename T>
inline std::string
export_object(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

}  // namespace _detail

}  // namespace cpp_dump
