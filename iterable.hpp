#pragma once

#include <string>

#include "export_tuple_like.hpp"
#include "export_xixo.hpp"

namespace cpp_dump {

template <typename T>
inline constexpr auto __is_iterable(T &&t)
    -> decltype(std::begin(t), std::end(t), std::true_type());
inline constexpr std::false_type __is_iterable(...);

template <typename T>
inline constexpr bool is_iterable =
    decltype(__is_iterable(std::declval<T>()))::value;

template <typename T>
inline constexpr bool is_iterable_like =
    is_iterable<T> || is_tuple_like<T> || is_xixo<T>;

template <typename T>
using iterable_elem_type = std::remove_const_t<
    std::remove_reference_t<decltype(*std::begin(std::declval<T>()))>>;

template <typename T>
inline bool is_empty_iterable(T &&t) {
  return std::distance(std::begin(t), std::end(t)) == 0;
}

}  // namespace cpp_dump