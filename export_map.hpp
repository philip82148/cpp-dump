#pragma once

#include <string>
#include <type_traits>

#include "iterable_like.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline const int MAX_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string, size_t);

template <typename T>
auto export_map(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_map<T>, std::string> {
  if (value.empty()) return "{ }";

  bool shift_indent =
      is_iterable_like<map_key_type<T>> || is_iterable_like<map_value_type<T>>;
  // 中身がiterable_likeのでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;
  std::string new_indent = indent + "  ";

rollback:
  std::string output = "{ ";
  bool is_first = true;
  for (auto elem_pair : value) {
    if (is_first) {
      is_first = false;
    } else {
      output += ", ";
    }

    if (shift_indent) {
      std::string prefix =
          "\n" + new_indent + export_var(elem_pair.first, new_indent) + ": ";

      output += prefix + export_var(elem_pair.second, new_indent,
                                    __last_line_length(prefix));
      continue;
    }

    std::string prefix = export_var(elem_pair.first, indent,
                                    first_line_length + output.length()) +
                         ": ";

    std::string elem_string =
        prefix +
        export_var(elem_pair.second, indent,
                   first_line_length + output.length() + prefix.length());
    if (!__has_lf(elem_string)) {
      output += elem_string;

      if (first_line_length + output.length() + 2 <= MAX_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }

  if (shift_indent) {
    output += "\n" + indent + "}";
  } else {
    output += " }";
  }

  return output;
}

}  // namespace cpp_dump
