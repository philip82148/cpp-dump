#pragma once

#include <string>
#include <type_traits>

#include "iterable_like.hpp"

namespace cpp_dump {

extern inline const int MAX_ITERABLE_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
auto export_map(T &&value, std::string indent)
    -> std::enable_if_t<is_map<T>, std::string> {
  if (value.empty()) return "{ }";

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  // 中身がiterable_likeのでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;
  std::string new_indent = indent + "  ";

rollback:
  std::string elems = "";
  for (auto elem_pair : value) {
    if (elems != "") elems += ", ";

    if (shift_indent) {
      elems += "\n" + new_indent + export_var(elem_pair.first, new_indent) +
               ": " + export_var(elem_pair.second, new_indent);
      continue;
    }

    std::string elem_string = export_var(elem_pair.first, indent) + ": " +
                              export_var(elem_pair.second, indent);
    if (elem_string.find("\n") == std::string::npos) {
      elems += elem_string;

      if (elems.length() + 4 <= MAX_ITERABLE_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }
  if (shift_indent) return "{" + elems + "\n" + indent + "}";

  return "{ " + elems + " }";
}

}  // namespace cpp_dump
