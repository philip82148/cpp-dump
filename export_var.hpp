#pragma once

#include <bits/stdc++.h>

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T, string = "");

}  // namespace ssk_debug

#include "export_iterable.hpp"
#include "export_map.hpp"
#include "export_tuple.hpp"
#include "iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename T>
inline auto export_var_real(T value, string) -> decltype(to_string(value)) {
  return to_string(value);
}

template <typename T,
          enable_if_t<iterable::is_iterable<T>, nullptr_t> = nullptr>
inline string export_var_real(T value, string indent) {
  return export_iterable(value, indent);
}

template <typename T1, typename T2>
string export_var_real(map<T1, T2> value, string indent) {
  return export_map(value, indent);
}

template <typename... Args>
inline string export_var_real(tuple<Args...> value, string indent) {
  return export_tuple(value, indent);
}

template <typename T1, typename T2>
inline string export_var_real(pair<T1, T2> value, string indent) {
  return export_var_real(make_tuple(value.first, value.second), indent);
}

string export_var_real(string, string);

inline string export_var_real(const char* value, string indent) {
  return export_var_real((string)(value), indent);
}

inline string export_var_real(char value, string) {
  return "'" + string{value} + "'";
}

inline string export_var_real(bool value, string) {
  return value ? "true" : "false";
}

template <typename T>
inline string export_var(T value, string indent) {
  return export_var_real(value, indent);
}

}  // namespace ssk_debug
