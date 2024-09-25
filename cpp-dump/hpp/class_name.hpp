/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <string>
#include <string_view>

#include "./escape_sequence.hpp"
#include "./options.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

namespace _detail {

// The return type must be a built-in type, otherwise we don't know how it will be stringified.
template <typename T>
const char* _get_classname_aux() {
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
std::string _get_classname() {
#if defined(__GNUC__) && !defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char* cpp_dump::_detail::_get_classname_aux() [with T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char *cpp_dump::_detail::_get_classname_aux() [T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(_MSC_VER)
  constexpr std::size_t prefix_length =
      std::string_view("const char *__cdecl cpp_dump::_detail::_get_classname_aux<").size();
  constexpr std::size_t suffix_length = std::string_view(">(void)").size();
#else
  constexpr std::size_t prefix_length = 0;
  constexpr std::size_t suffix_length = 0;
#endif

  std::string_view func_name = _get_classname_aux<remove_cvref<T>>();
  std::string class_name(
      func_name, prefix_length, func_name.size() - prefix_length - suffix_length
  );

#if defined(_MSC_VER)
  class_name = replace_string(class_name, "struct ", "");
  class_name = replace_string(class_name, "class ", "");
  class_name = replace_string(class_name, "enum ", "");
#endif

  return class_name;
}

inline std::string styled_classname_str(std::string_view type_name) {
  std::string styled(type_name);

  if (options::classname_style & flags::classname_style::no_namespace) {
    std::string no_ns;
    no_ns.swap(styled);
    std::reverse(no_ns.begin(), no_ns.end());
    int gt_count = 0;
    for (std::size_t i = 0; i < no_ns.size(); ++i) {
      if (no_ns[i] == '>') {
        ++gt_count;
      } else if (no_ns[i] == '<') {
        --gt_count;
      } else if (gt_count == 0) {
        if (no_ns[i] == ':') {
          no_ns.erase(i);
          break;
        }
      }
    }
    std::reverse(no_ns.begin(), no_ns.end());
    styled.swap(no_ns);
  }

  if (options::classname_style & flags::classname_style::no_temp_args) {
    std::string no_args;
    int lt_count = 0;
    for (auto c : styled) {
      if (c == '<') {
        ++lt_count;
      } else if (c == '>') {
        --lt_count;
      } else if (lt_count == 0) {
        no_args.push_back(c);
      }
    }
    styled.swap(no_args);
  }

  if (options::classname_style & flags::classname_style::max_width_20 && styled.size() > 20)
    return es::class_name(styled.substr(0, 17)) + es::op("...");

  return es::class_name(styled);
}

// Currently, used only by export_exception() and CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC()
template <typename T>
inline std::string styled_classname() {
  static std::string class_name = _get_classname<T>();
  return styled_classname_str(class_name);
}

}  // namespace _detail

}  // namespace cpp_dump
