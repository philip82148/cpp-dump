#pragma once

#include <bits/stdc++.h>

#include "iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename T>
string export_var(T);

template <typename... Args>
string export_tuple(tuple<Args...> value);

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...> value);

template <typename... Args>
string export_tuple(tuple<Args...> value) {
  return "( " + export_tuple_aux<0, tuple_size<tuple<Args...>>::value>(value) +
         " )";
}

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...> value) {
  if constexpr (i < size - 1) {
    return export_var(get<i>(value)) + ", " +
           export_tuple_aux<i + 1, size>(value);
  } else {
    return export_var(get<i>(value));
  }
}

}  // namespace ssk_debug
