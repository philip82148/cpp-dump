#pragma once

#include <bits/stdc++.h>

#include "export_var.hpp"

namespace ssk_debug {

using namespace std;

template <typename T1, typename T2>
string export_map(map<T1, T2>, string);

template <typename T1, typename T2>
string export_map(map<T1, T2> value, string indent) {
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

}  // namespace ssk_debug
