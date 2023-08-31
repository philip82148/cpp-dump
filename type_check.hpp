/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace cpp_dump {

template <typename T>
inline constexpr auto _is_iterable(const T &t)
    -> decltype(std::begin(t), std::end(t), std::true_type());
inline constexpr std::false_type _is_iterable(...);

template <typename T>
inline constexpr bool is_iterable =
    decltype(_is_iterable(std::declval<std::remove_cvref_t<T>>()))::value;

template <typename T>
using iterable_elem_type =
    std::remove_cvref_t<decltype(*std::begin(std::declval<std::remove_cvref_t<T>>()))>;

template <typename T>
inline bool is_empty_iterable(const T &t) {
  return std::distance(std::begin(t), std::end(t)) == 0;
}

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<std::remove_cvref_t<T>, std::string>;

template <typename T>
inline constexpr bool _is_map = false;
template <typename T1, typename T2>
inline constexpr bool _is_map<std::map<T1, T2>> = true;
template <typename T1, typename T2>
inline constexpr bool _is_map<std::unordered_map<T1, T2>> = true;

template <typename T>
inline constexpr bool is_map = _is_map<std::remove_cvref_t<T>>;

template <typename T>
using map_key_type = std::remove_cvref_t<decltype(std::declval<iterable_elem_type<T>>().first)>;

template <typename T>
using map_value_type = std::remove_cvref_t<decltype(std::declval<iterable_elem_type<T>>().second)>;

template <typename T>
inline constexpr bool _is_set = false;
template <typename T>
inline constexpr bool _is_set<std::set<T>> = true;
template <typename T>
inline constexpr bool _is_set<std::unordered_set<T>> = true;

template <typename T>
inline constexpr bool is_set = _is_set<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename... Args>
inline constexpr bool _is_tuple = false;
template <typename... Args>
inline constexpr bool _is_tuple<std::tuple<Args...>> = true;

template <typename T>
inline constexpr bool is_tuple = _is_tuple<std::remove_cvref_t<T>>;

template <typename... Args>
inline constexpr bool _is_pair = false;
template <typename T1, typename T2>
inline constexpr bool _is_pair<std::pair<T1, T2>> = true;

template <typename T>
inline constexpr bool is_pair = _is_pair<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr bool is_tuple_like = is_tuple<T> || is_pair<T>;

template <typename T>
inline constexpr bool _is_queue = false;
template <typename T>
inline constexpr bool _is_queue<std::queue<T>> = true;

template <typename T>
inline constexpr bool is_queue = _is_queue<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr bool _is_priority_queue = false;
template <typename T>
inline constexpr bool _is_priority_queue<std::priority_queue<T>> = true;

template <typename T>
inline constexpr bool is_priority_queue = _is_priority_queue<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr bool _is_stack = false;
template <typename T>
inline constexpr bool _is_stack<std::stack<T>> = true;

template <typename T>
inline constexpr bool is_stack = _is_stack<std::remove_cvref_t<T>>;

template <typename T>
inline constexpr bool is_xixo = is_queue<T> || is_priority_queue<T> || is_stack<T>;

template <typename T>
inline constexpr bool is_object = (std::is_union_v<std::remove_cvref_t<T>> ||
                                   std::is_class_v<std::remove_cvref_t<T>>)&&!is_string<T> &&
                                  !is_set<T> && !is_map<T> && !is_tuple_like<T> &&
                                  !is_container<T> && !is_xixo<T>;

template <typename T>
inline constexpr bool is_iterable_like =
    is_iterable<T> || is_tuple_like<T> || is_xixo<T> || is_object<T>;

}  // namespace cpp_dump
