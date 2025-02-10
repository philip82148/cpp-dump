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

inline std::string format_class_name(std::string_view class_name) {
  std::string raw_output(class_name);
  if (options::class_name_format & flags::class_name_format::no_temp_args) {
    std::string raw_output_wo_arg;
    int lt_count = 0;
    for (auto c : raw_output) {
      if (c == '<') {
        ++lt_count;
      } else if (c == '>') {
        --lt_count;
      } else if (lt_count == 0) {
        raw_output_wo_arg.push_back(c);
      }
    }
    raw_output.swap(raw_output_wo_arg);
  }
  if (options::class_name_format & flags::class_name_format::no_namespace) {
    std::string raw_output_wo_namespace;
    for (int i = static_cast<int>(raw_output.size() - 1); i >= 0; --i) {
      if (raw_output[i] != ':') {
        // append class name
        raw_output_wo_namespace.push_back(raw_output[i]);
        continue;
      }

      // skip namespace
      int gt_count = 0;
      for (--i; i >= 0; --i) {
        if (raw_output[i] == '>') {
          ++gt_count;
        } else if (raw_output[i] == '<') {
          --gt_count;
          if (gt_count < 0) {
            raw_output_wo_namespace.push_back(raw_output[i]);
            break;
          }
        }
      }
    }
    std::reverse(raw_output_wo_namespace.begin(), raw_output_wo_namespace.end());
    raw_output.swap(raw_output_wo_namespace);
  }
  if (options::class_name_format & flags::class_name_format::max_width_20
      && raw_output.size() > 20) {
    return es::class_name(raw_output.substr(0, 17)) + es::op("...");
  }
  return es::class_name(raw_output);
}

// The return type must be a built-in type, otherwise we don't know how it will be stringified.
template <typename T>
const char* _get_class_name_aux() {
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
std::string _get_class_name() {
#if defined(__GNUC__) && !defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char* cpp_dump::_detail::_get_class_name_aux() [with T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char *cpp_dump::_detail::_get_class_name_aux() [T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(_MSC_VER)
  constexpr std::size_t prefix_length =
      std::string_view("const char *__cdecl cpp_dump::_detail::_get_class_name_aux<").size();
  constexpr std::size_t suffix_length = std::string_view(">(void)").size();
#else
  constexpr std::size_t prefix_length = 0;
  constexpr std::size_t suffix_length = 0;
#endif

  std::string_view func_name = _get_class_name_aux<remove_cvref<T>>();
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

// Currently, used only by export_exception(), CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(), and
// CPP_DUMP_DEFINE_EXPORT_ENUM_GENERIC()
template <typename T>
inline std::string format_class_name_auto() {
  static std::string class_name = _get_class_name<T>();
  return format_class_name(class_name);
}

}  // namespace _detail

}  // namespace cpp_dump
