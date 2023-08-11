#pragma once

#include <bits/stdc++.h>

#include "export_vector.hpp"

namespace ssk_debug {

using namespace std;

template <typename T>
inline string export_var(T value) {
  return to_string(value);
}

template <typename T>
inline string export_var(vector<T> value) {
  return export_vector(value, "");
}

template <>
string export_var(string);

template <>
inline string export_var(const char* value) {
  return export_var(string(value));
}

template <>
inline string export_var(bool value) {
  return value ? "true" : "false";
}

}  // namespace ssk_debug