#pragma once

#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>

namespace cpp_dump {

extern inline const int MAX_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
inline constexpr bool __is_set = false;
template <typename T>
inline constexpr bool __is_set<std::set<T>> = true;
template <typename T>
inline constexpr bool __is_set<std::unordered_set<T>> = true;

template <typename T>
inline constexpr bool is_set = __is_set<std::remove_reference_t<T>>;

template <typename T>
auto export_set(T &&value, std::string indent)
    -> std::enable_if_t<is_set<T>, std::string> {
  if (value.empty()) return "{ }";

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

      if (elems.length() + 4 <= MAX_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }
  if (shift_indent) return "{" + elems + "\n" + indent + "}";

  return "{ " + elems + " }";
}

}  // namespace cpp_dump
