#pragma once

#include <bits/stdc++.h>

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T);

template <typename T1, typename T2>
string export_map(map<T1, T2> value);

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...> value);

template <typename T1, typename T2>
string export_map(map<T1, T2> value) {
  string content = "";

  for (auto pair : value) {
    if (content != "") content += ", ";
    content += export_var(pair.first) + ": " + export_var(pair.second);
  }

  return "{ " + content + " }";
}

}  // namespace ssk_debug
