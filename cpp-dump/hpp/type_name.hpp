/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>

#include "./escape_sequence.hpp"
#include "./options.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

// The return type must be a built-in type, otherwise we don't know how it will be stringified.
template <typename T>
const char* _get_typename_aux() {
#if defined(__GNUC__) && !defined(__clang__)
  return __PRETTY_FUNCTION__;
#elif defined(__clang__)
  return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
  return __FUNCSIG__;
#else
  return "";
#endif
}

template <typename T>
std::string _get_typename() {
#if defined(__GNUC__) && !defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char* cpp_dump::_detail::_get_typename_aux() [with T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char *cpp_dump::_detail::_get_typename_aux() [T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(_MSC_VER)
  constexpr std::size_t prefix_length =
      std::string_view("const char *__cdecl cpp_dump::_detail::_get_typename_aux<").size();
  constexpr std::size_t suffix_length = std::string_view(">(void)").size();
#else
  constexpr std::size_t prefix_length = 0;
  constexpr std::size_t suffix_length = 0;
#endif

  std::string_view func_name = _get_typename_aux<_remove_cvref<T>>();
  std::string type_name(func_name, prefix_length, func_name.size() - prefix_length - suffix_length);

#if defined(_MSC_VER)
  type_name = replace_string(type_name, "struct ", "");
  type_name = replace_string(type_name, "class ", "");
#endif

  return type_name;
}

inline std::string styled_typename_str(std::string_view type_name) {
  switch (options::typename_style) {
    case types::typename_style_t::no_temp_args: {
      std::string typename_no_arg;
      int lt_count = 0;
      for (auto c : type_name) {
        if (c == '<') {
          ++lt_count;
        } else if (c == '>') {
          --lt_count;
        } else if (lt_count == 0) {
          typename_no_arg.push_back(c);
        }
      }
      return es::class_name(typename_no_arg);
    }
    case types::typename_style_t::maximum20: {
      if (type_name.size() <= 20) return es::class_name(type_name);
      return es::class_name(type_name.substr(0, 17)) + es::op("...");
    }
    default: {
      return es::class_name(type_name);
    }
  }
}

// Currently, used only by export_exception() and CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC()
template <typename T>
inline std::string styled_typename() {
  static std::string type_name = _get_typename<T>();
  return styled_typename_str(type_name);
}

}  // namespace _detail

}  // namespace cpp_dump
