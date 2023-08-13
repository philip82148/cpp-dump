#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace cpp_dump {

extern inline const int MAX_LINE_WIDTH;

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
inline constexpr bool __is_map = false;
template <typename T1, typename T2>
inline constexpr bool __is_map<std::map<T1, T2>> = true;
template <typename T1, typename T2>
inline constexpr bool __is_map<std::unordered_map<T1, T2>> = true;

template <typename T>
inline constexpr bool is_map = __is_map<std::remove_reference_t<T>>;

template <typename T>
auto export_map(T &&value, std::string indent)
    -> std::enable_if_t<is_map<T>, std::string> {
  if (value.empty()) return "{ }";

  bool shift_indent = false;
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

      if (elems.length() + 4 <= MAX_LINE_WIDTH) continue;
    }

    shift_indent = true;
    goto rollback;
  }
  if (shift_indent) return "{" + elems + "\n" + indent + "}";

  return "{ " + elems + " }";
}

}  // namespace cpp_dump
