#pragma once

#include <bits/stdc++.h>

#include "export_tuple.hpp"
#include "export_xixo.hpp"

namespace cpp_dump {

using namespace std;

template <typename T>
inline constexpr auto __is_iterable(T&& t)
    -> decltype(std::begin(t), std::end(t), true_type());
inline constexpr false_type __is_iterable(...);

template <typename T>
inline constexpr bool is_iterable =
    decltype(__is_iterable(declval<T>()))::value;

template <typename T>
inline constexpr bool is_iterable_like =
    is_iterable<T> || is_tuple<T> || is_xixo<T>;

template <typename T>
using iterable_elem_type =
    remove_const_t<remove_reference_t<decltype(*std::begin(declval<T>()))>>;

template <typename T>
inline bool is_empty_iterable(T&& t) {
  return std::distance(std::begin(t), std::end(t)) == 0;
}

}  // namespace cpp_dump