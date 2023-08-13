#pragma once

#include <bits/stdc++.h>

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename... Args>
constexpr bool is_tuple = false;
template <typename T1, typename T2>
constexpr bool is_tuple<pair<T1, T2>> = true;
template <typename... Args>
constexpr bool is_tuple<tuple<Args...>> = true;

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...> value, string indent) {
  if constexpr (i < size - 1) {
    return export_var(get<i>(value), indent) + ", " +
           export_tuple_aux<i + 1, size>(value, indent);
  } else {
    return export_var(get<i>(value), indent);
  }
}

template <typename... Args>
string export_tuple(tuple<Args...> value, string indent) {
  return "( " +
         export_tuple_aux<0, tuple_size<tuple<Args...>>::value>(value, indent) +
         " )";
}

template <typename T1, typename T2>
inline string export_tuple(pair<T1, T2> value, string indent) {
  return export_tuple(make_tuple(value.first, value.second), indent);
}

}  // namespace cpp_dump
