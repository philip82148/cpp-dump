#pragma once

#include <bits/stdc++.h>

#include "export_iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T value);

template <typename T>
inline auto export_var_real(T value) -> decltype(to_string(value)) {
  return to_string(value);
}

template <typename T,
          enable_if_t<iterable::is_iterable<T>, nullptr_t> = nullptr>
inline string export_var_real(T value) {
  return export_iterable(value, "");
}

string export_var_real(string);

inline string export_var_real(const char* value) {
  return export_var_real((string)(value));
}

inline string export_var_real(char value) { return "'" + string{value} + "'"; }

inline string export_var_real(bool value) { return value ? "true" : "false"; }

template <typename T>
inline string export_var(T value) {
  return export_var_real(value);
}

}  // namespace ssk_debug
