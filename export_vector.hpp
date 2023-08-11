#pragma once

#include <bits/stdc++.h>

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T);

template <typename T>
string export_vector(vector<T>, string);

template <typename T>
string export_vector(vector<vector<T>>, string);

template <typename T>
string export_vector(vector<T> value, string) {
  string content = "";

  for (T v : value) {
    if (content != "") content += ", ";

    content += export_var(v);
  }

  return "[" + content + "]";
}

template <typename T>
string export_vector(vector<vector<T>> value, string indent) {
  string content = "";

  string newIndent = indent + "  ";
  for (vector<T> v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_vector(v, newIndent);
  }
  if (content != "") content += "\n" + indent;

  return "[" + content + "]";
}

}  // namespace ssk_debug
