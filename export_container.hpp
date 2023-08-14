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
auto export_container(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_container<T>, std::string> {
  if (is_empty_iterable(value)) return "[ ]";

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  // 中身がiterable_likeのでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;
  std::string new_indent = indent + "  ";

rollback:
  std::string elems = "";
  for (auto elem_value : value) {
    if (elems != "") elems += ", ";

    if (shift_indent) {
      elems += "\n" + new_indent + export_var(elem_value, new_indent);
      continue;
    }

    std::string elem_string =
        export_var(elem_value, indent, first_line_length + elems.length() + 2);
    if (!__has_lf(elem_string)) {
      elems += elem_string;

      if (first_line_length + elems.length() + 4 <= MAX_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }
  if (shift_indent) return "[" + elems + "\n" + indent + "]";

  return "[ " + elems + " ]";
}

}  // namespace cpp_dump
