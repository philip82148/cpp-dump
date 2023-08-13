#pragma once

#include <string>
#include <type_traits>

#include "export_map.hpp"
#include "export_set.hpp"
#include "iterable.hpp"

namespace cpp_dump {

template <typename T>
std::string export_var(T, std::string);

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_map<T> && !is_set<T>;

template <typename T>
inline auto __export_empty_container(std::string)
    -> std::enable_if_t<!is_container<iterable_elem_type<T>>, std::string> {
  return "[ ]";
}

template <typename T>
inline auto __export_empty_container(std::string indent)
    -> std::enable_if_t<is_container<iterable_elem_type<T>>, std::string> {
  return "[ " + __export_empty_container<iterable_elem_type<T>>(indent) + " ]";
}

template <typename T>
auto export_container(T value, std::string indent) -> std::enable_if_t<
    is_container<T> && !is_iterable_like<iterable_elem_type<T>>, std::string> {
  // 中身が空の時
  if (is_empty_iterable(value)) return __export_empty_container<T>(indent);

  std::string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "[ " + content + " ]";
}

template <typename T>
auto export_container(T value, std::string indent)
    -> std::enable_if_t<is_iterable_like<iterable_elem_type<T>>, std::string> {
  // 中身が空の時
  if (is_empty_iterable(value)) return __export_empty_container<T>(indent);

  std::string content = "";

  std::string newIndent = indent + "  ";
  for (auto v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_var(v, newIndent);
  }
  content += "\n" + indent;

  return "[" + content + "]";
}

}  // namespace cpp_dump
