#pragma once

#include <bits/stdc++.h>

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename T>
constexpr bool is_set = false;
template <typename T>
constexpr bool is_set<set<T>> = true;
template <typename T>
constexpr bool is_set<unordered_set<T>> = true;

template <typename T>
auto export_set(T value, string indent) -> enable_if_t<is_set<T>, string> {
  if (value.size() == 0) return "{ }";

  string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "{ " + content + " }";
}

}  // namespace cpp_dump
