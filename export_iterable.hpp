#pragma once

#include <bits/stdc++.h>

#include "export_var.hpp"
#include "iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename T,
          enable_if_t<iterable::is_iterable<T> &&
                          !iterable::is_iterable<iterable::ChildType<T>>,
                      nullptr_t> = nullptr>
string export_iterable(T, string);

template <typename T, enable_if_t<iterable::is_iterable<iterable::ChildType<T>>,
                                  nullptr_t> = nullptr>
string export_iterable(T, string);

template <typename T>
string export_empty_iterable(string);

template <typename T,
          enable_if_t<iterable::is_iterable<T> &&
                          !iterable::is_iterable<iterable::ChildType<T>>,
                      nullptr_t>>
string export_iterable(T value, string indent) {
  // 中身が空の時
  if (iterable::size(value) == 0) return export_empty_iterable<T>(indent);

  string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "[ " + content + " ]";
}

template <typename T,
          enable_if_t<iterable::is_iterable<iterable::ChildType<T>>, nullptr_t>>
string export_iterable(T value, string indent) {
  // 中身が空の時
  if (iterable::size(value) == 0) return export_empty_iterable<T>(indent);

  string content = "";

  string newIndent = indent + "  ";
  for (auto v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_var(v, newIndent);
  }
  content += "\n" + indent;

  return "[" + content + "]";
}

template <typename T>
string export_empty_iterable(string indent) {
  using ChildType = iterable::ChildType<T>;

  if constexpr (iterable::is_iterable<ChildType>) {
    return "[ " + export_empty_iterable<ChildType>(indent) + " ]";
  } else {
    return "[ ]";
  }
}

}  // namespace ssk_debug
