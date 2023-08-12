#pragma once

#include <bits/stdc++.h>

#include "export_var.hpp"
#include "iterable.hpp"

namespace ssk_debug {

using namespace std;

template <typename... Args>
string export_tuple(tuple<Args...>, string);

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...>, string);

template <typename... Args>
string export_tuple(tuple<Args...> value, string indent) {
  return "( " +
         export_tuple_aux<0, tuple_size<tuple<Args...>>::value>(value, indent) +
         " )";
}

template <const int i, const int size, typename... Args>
string export_tuple_aux(tuple<Args...> value, string indent) {
  if constexpr (i < size - 1) {
    return export_var(get<i>(value), indent) + ", " +
           export_tuple_aux<i + 1, size>(value, indent);
  } else {
    return export_var(get<i>(value), indent);
  }
}

}  // namespace ssk_debug
