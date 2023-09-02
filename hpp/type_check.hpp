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
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace cpp_dump {

namespace _detail {

template <typename T>
using _remove_cref = std::remove_const_t<std::remove_reference_t<T>>;

template <typename T>
auto _is_iterable(const T &t) -> decltype(std::begin(t), std::end(t), std::true_type());
std::false_type _is_iterable(...);

template <typename T>
inline bool is_empty_iterable(const T &t) {
  return std::distance(std::begin(t), std::end(t)) == 0;
}

}  // namespace _detail

template <typename T>
inline constexpr bool is_iterable = decltype(_detail::_is_iterable(std::declval<T>()))::value;

template <typename T>
using iterable_elem_type = _detail::_remove_cref<decltype(*std::begin(std::declval<T>()))>;

template <typename T>
inline constexpr bool is_arithmetic = std::is_arithmetic_v<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_string =
    std::is_convertible_v<T, std::string_view> && !std::is_null_pointer_v<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_pointer = (std::is_pointer_v<_detail::_remove_cref<T>> && !is_string<T>) ||
                                   std::is_null_pointer_v<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_void_pointer = std::is_same_v<_detail::_remove_cref<T>, void *> ||
                                        std::is_same_v<_detail::_remove_cref<T>, const void *> ||
                                        std::is_null_pointer_v<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_map = false;
template <typename T1, typename T2>
inline constexpr bool _is_map<std::map<T1, T2>> = true;
template <typename T1, typename T2>
inline constexpr bool _is_map<std::unordered_map<T1, T2>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_map = _detail::_is_map<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_set = false;
template <typename T>
inline constexpr bool _is_set<std::set<T>> = true;
template <typename T>
inline constexpr bool _is_set<std::unordered_set<T>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_set = _detail::_is_set<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

namespace _detail {

template <typename>
inline constexpr bool _is_tuple = false;
template <typename... Args>
inline constexpr bool _is_tuple<std::tuple<Args...>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_tuple = _detail::_is_tuple<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_pair = false;
template <typename T1, typename T2>
inline constexpr bool _is_pair<std::pair<T1, T2>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_pair = _detail::_is_pair<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_tuple_like = is_tuple<T> || is_pair<T>;

namespace _detail {

template <typename>
inline constexpr bool _is_queue = false;
template <typename T>
inline constexpr bool _is_queue<std::queue<T>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_queue = _detail::_is_queue<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_priority_queue = false;
template <typename T>
inline constexpr bool _is_priority_queue<std::priority_queue<T>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_priority_queue = _detail::_is_priority_queue<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_stack = false;
template <typename T>
inline constexpr bool _is_stack<std::stack<T>> = true;

}  // namespace _detail

template <typename T>
inline constexpr bool is_stack = _detail::_is_stack<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_xixo = is_queue<T> || is_priority_queue<T> || is_stack<T>;

namespace _detail {

template <typename>
inline constexpr bool _is_exportable_object = false;

}

template <typename T>
inline constexpr bool is_exportable_object =
    _detail::_is_exportable_object<_detail::_remove_cref<T>>;

namespace _detail {

template <typename>
inline constexpr bool _is_exportable_enum = false;

}

template <typename T>
inline constexpr bool is_exportable_enum = _detail::_is_exportable_enum<_detail::_remove_cref<T>>;

template <typename T>
inline constexpr bool is_iterable_like =
    is_iterable<T> || is_tuple_like<T> || is_xixo<T> || is_exportable_object<T>;

template <typename T>
inline constexpr bool is_exportable =
    is_arithmetic<T> || is_string<T> || is_pointer<T> || is_map<T> || is_set<T> ||
    is_container<T> || is_tuple_like<T> || is_xixo<T> || is_exportable_object<T> ||
    is_exportable_enum<T>;

}  // namespace cpp_dump
