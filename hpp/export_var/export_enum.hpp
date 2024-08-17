/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "../escape_sequence.hpp"
#include "../expand_va_macro.hpp"
#include "../export_command/export_command.hpp"
#include "../options.hpp"
#include "../type_check.hpp"

#define _p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM(member)                                                 \
  { member, #member }

/**
 * Make cpp_dump::export_var() support enum TYPE.
 */
#define CPP_DUMP_DEFINE_EXPORT_ENUM(TYPE, ...)                                                                       \
  namespace cpp_dump {                                                                                               \
                                                                                                                     \
  namespace _detail {                                                                                                \
                                                                                                                     \
  template <>                                                                                                        \
  inline constexpr bool _is_exportable_enum<TYPE> = true;                                                            \
                                                                                                                     \
  template <>                                                                                                        \
  inline std::string                                                                                                 \
  export_enum(const TYPE &enum_const, const std::string &, std::size_t, std::size_t, bool, const export_command &) { \
    static const std::unordered_map<TYPE, std::string_view> enum_to_string{                                          \
        _p_CPP_DUMP_EXPAND_VA(_p_CPP_DUMP_EXPAND_FOR_EXPORT_ENUM, __VA_ARGS__)};                                     \
    return enum_to_string.count(enum_const)                                                                          \
               ? es::enumerator(enum_to_string.at(enum_const))                                                       \
               : es::class_name(#TYPE) + es::op("::") + es::unsupported("?");                                        \
  }                                                                                                                  \
                                                                                                                     \
  } /* namespace _detail */                                                                                          \
                                                                                                                     \
  }  // namespace cpp_dump

namespace cpp_dump {

namespace _detail {

template <typename T>
inline std::string
export_enum(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

}  // namespace _detail

}  // namespace cpp_dump

CPP_DUMP_DEFINE_EXPORT_ENUM(
    cpp_dump::types::es_style_t,
    cpp_dump::types::es_style_t::no_es,
    cpp_dump::types::es_style_t::original,
    cpp_dump::types::es_style_t::by_syntax
);
CPP_DUMP_DEFINE_EXPORT_ENUM(
    cpp_dump::types::cont_indent_style_t,
    cpp_dump::types::cont_indent_style_t::minimal,
    cpp_dump::types::cont_indent_style_t::when_nested,
    cpp_dump::types::cont_indent_style_t::when_non_tuples_nested,
    cpp_dump::types::cont_indent_style_t::always
);
