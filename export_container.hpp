#pragma once

#include <string>
#include <type_traits>

#include "export_map.hpp"
#include "export_set.hpp"
#include "iterable.hpp"

namespace cpp_dump {

extern inline const int MAX_ITERABLE_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<T, std::string>;

template <typename T>
inline constexpr bool is_container =
    is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename T>
auto export_container(T &&value, std::string indent)
    -> std::enable_if_t<is_container<T>, std::string> {
  if (is_empty_iterable(value)) return "[ ]";

  bool shift_indent = false;
  std::string new_indent = indent + "  ";

rollback:
  std::string elems = "";
  for (auto elem_value : value) {
    if (elems != "") elems += ", ";

    if (shift_indent) {
      elems += "\n" + new_indent + export_var(elem_value, new_indent);
      continue;
    }

    std::string elem_string = export_var(elem_value, indent);
    if (elem_string.find("\n") == std::string::npos) {
      elems += elem_string;

      if (elems.length() + 4 <= MAX_ITERABLE_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }
  if (shift_indent) return "[" + elems + "\n" + indent + "]";

  return "[ " + elems + " ]";
}

// template <typename T>
// auto export_container(T value, std::string indent)
//     -> std::enable_if_t<is_iterable_like<iterable_elem_type<T>>, std::string>
//     {
//   if (is_empty_iterable(value)) return "[ ]";

//   std::string content = "";

//   std::string newIndent = indent + "  ";
//   for (auto v : value) {
//     if (content != "") content += ",";

//     content += "\n" + newIndent + export_var(v, newIndent);
//   }
//   content += "\n" + indent;

//   return "[" + content + "]";
// }

}  // namespace cpp_dump
