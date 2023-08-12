#pragma once

#include <bits/stdc++.h>

#include "iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T);

template <typename T,
          enable_if_t<iterable::is_iterable<T> &&
                          !iterable::is_iterable<iterable::ChildType<T>>,
                      nullptr_t> = nullptr>
string export_iterable(T, string);

template <typename T, enable_if_t<iterable::is_iterable<iterable::ChildType<T>>,
                                  nullptr_t> = nullptr>
string export_iterable(T, string);

template <typename T,
          enable_if_t<iterable::is_iterable<T> &&
                          !iterable::is_iterable<iterable::ChildType<T>>,
                      nullptr_t>>
string export_iterable(T value, string) {
  string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v);
  }

  return "[" + content + "]";
}

template <typename T,
          enable_if_t<iterable::is_iterable<iterable::ChildType<T>>, nullptr_t>>
string export_iterable(T value, string indent) {
  string content = "";

  string newIndent = indent + "  ";
  for (auto v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_iterable(v, newIndent);
  }
  if (content != "") content += "\n" + indent;

  return "[" + content + "]";
}

}  // namespace ssk_debug
