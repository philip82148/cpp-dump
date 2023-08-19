#pragma once

#include <string>
#include <type_traits>

#include "type_check.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline const int max_line_width;

template <typename T>
std::string export_var(T &&, std::string, size_t);

template <typename T>
auto export_container(T &&value, std::string indent, size_t last_line_length)
    -> std::enable_if_t<is_container<T>, std::string> {
  if (is_empty_iterable(value)) return "[ ]";

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  // 中身がiterable_likeのでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;
  std::string new_indent = indent + "  ";

rollback:
  std::string output = "[ ";
  bool is_first = true;
  for (auto elem : value) {
    if (is_first) {
      is_first = false;
    } else {
      output += ", ";
    }

    if (shift_indent) {
      output += "\n" + new_indent + export_var(elem, new_indent);
      continue;
    }

    std::string elem_string = export_var(elem, indent, last_line_length + output.length());
    if (!_has_lf(elem_string)) {
      output += elem_string;

      if (last_line_length + (output + " ]").length() <= max_line_width) continue;
    }

    shift_indent = true;
    goto rollback;
  }

  if (shift_indent) {
    output += "\n" + indent + "]";
  } else {
    output += " ]";
  }

  return output;
}

}  // namespace cpp_dump
