#pragma once

#include <bits/stdc++.h>

#include "export_map.hpp"
#include "export_set.hpp"
#include "iterable.hpp"

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_map<T> && !is_set<T>;

template <typename T>
inline auto __export_empty_container(string)
    -> enable_if_t<!is_container<iterable_elem_type<T>>, string> {
  return "[ ]";
}

template <typename T>
inline auto __export_empty_container(string indent)
    -> enable_if_t<is_container<iterable_elem_type<T>>, string> {
  return "[ " + __export_empty_container<iterable_elem_type<T>>(indent) + " ]";
}

template <typename T>
auto export_container(T value, string indent)
    -> enable_if_t<is_container<T> && !is_iterable_like<iterable_elem_type<T>>,
                   string> {
  // 中身が空の時
  if (is_empty_iterable(value)) return __export_empty_container<T>(indent);

  string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "[ " + content + " ]";
}

template <typename T>
auto export_container(T value, string indent)
    -> enable_if_t<is_iterable_like<iterable_elem_type<T>>, string> {
  // 中身が空の時
  if (is_empty_iterable(value)) return __export_empty_container<T>(indent);

  string content = "";

  string newIndent = indent + "  ";
  for (auto v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_var(v, newIndent);
  }
  content += "\n" + indent;

  return "[" + content + "]";
}

}  // namespace cpp_dump
