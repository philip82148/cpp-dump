#pragma once

#include <bits/stdc++.h>

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename T>
constexpr bool is_map = false;
template <typename T1, typename T2>
constexpr bool is_map<map<T1, T2>> = true;
template <typename T1, typename T2>
constexpr bool is_map<unordered_map<T1, T2>> = true;

template <typename T>
auto export_map(T value, string indent) -> enable_if_t<is_map<T>, string> {
  if (value.size() == 0) return "{ }";

  string content = "";

  string newIndent = indent + "  ";
  for (auto pair : value) {
    if (content != "") content += ",";
    content += "\n" + newIndent + export_var(pair.first, newIndent) + ": " +
               export_var(pair.second, newIndent);
  }
  content += "\n" + indent;

  return "{" + content + "}";
}

}  // namespace cpp_dump
