/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <bitset>
#include <complex>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>

namespace cpp_dump {

namespace _detail {

template <typename T>
using _remove_cref = std::remove_const_t<std::remove_reference_t<T>>;

template <typename T>
auto _begin(const T &t, int) -> decltype(begin(t)) {
  return begin(t);
}

template <typename T>
auto _begin(const T &t, long) -> decltype(std::begin(t)) {
  return std::begin(t);
}

template <typename T>
auto _end(const T &t, int) -> decltype(end(t)) {
  return end(t);
}

template <typename T>
auto _end(const T &t, long) -> decltype(std::end(t)) {
  return std::end(t);
}

template <typename T>
auto _is_iterable(int)
    -> decltype(_begin(std::declval<T>(), 0) != _end(std::declval<T>(), 0), std::true_type());
template <typename>
std::false_type _is_iterable(long);

template <typename T>
inline constexpr bool is_iterable = decltype(_is_iterable<T>(0))::value;

template <typename T>
using iterable_elem_type = _remove_cref<decltype(*_begin(std::declval<T>(), 0))>;

template <typename T>
inline bool is_empty_iterable(const T &t) {
  return !(_begin(t, 0) != _end(t, 0));
}

template <typename T>
inline constexpr bool is_arithmetic = std::is_arithmetic_v<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_null_pointer = std::is_null_pointer_v<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<T, std::string_view> && !is_null_pointer<T>;

template <typename>
inline constexpr bool _is_smart_pointer = false;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::unique_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::shared_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::weak_ptr<Args...>> = true;

template <typename T>
inline constexpr bool is_smart_pointer = _is_smart_pointer<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_pointer = (std::is_pointer_v<_remove_cref<T>> && !is_string<T>) ||
                                   is_null_pointer<T> || is_smart_pointer<T>;

template <typename T>
auto _remove_pointer(int) -> std::enable_if_t<is_smart_pointer<T>, typename T::element_type>;
template <typename T>
auto _remove_pointer(long) -> std::remove_pointer_t<T>;

template <typename T>
using remove_pointer = decltype(_remove_pointer<_remove_cref<T>>(0));

template <typename>
inline constexpr bool _is_optional = false;
template <typename... Args>
inline constexpr bool _is_optional<std::optional<Args...>> = true;
template <>
inline constexpr bool _is_optional<std::nullopt_t> = true;

template <typename T>
inline constexpr bool is_optional = _is_optional<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_map = false;
template <typename... Args>
inline constexpr bool _is_map<std::map<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_map<std::unordered_map<Args...>> = true;

template <typename>
inline constexpr bool _is_multimap = false;
template <typename... Args>
inline constexpr bool _is_multimap<std::multimap<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_multimap<std::unordered_multimap<Args...>> = true;

template <typename T>
inline constexpr bool is_multimap = _is_multimap<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_map = _is_map<_remove_cref<T>> || is_multimap<T>;

template <typename>
inline constexpr bool _is_set = false;
template <typename... Args>
inline constexpr bool _is_set<std::set<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_set<std::unordered_set<Args...>> = true;

template <typename>
inline constexpr bool _is_multiset = false;
template <typename... Args>
inline constexpr bool _is_multiset<std::multiset<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_multiset<std::unordered_multiset<Args...>> = true;

template <typename T>
inline constexpr bool is_multiset = _is_multiset<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_set = _is_set<_remove_cref<T>> || is_multiset<T>;

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename>
inline constexpr bool _is_tuple_like = false;
template <typename... Args>
inline constexpr bool _is_tuple_like<std::tuple<Args...>> = true;
template <typename T1, typename T2>
inline constexpr bool _is_tuple_like<std::pair<T1, T2>> = true;

template <typename T>
inline constexpr bool is_tuple_like = _is_tuple_like<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_xixo = false;
template <typename... Args>
inline constexpr bool _is_xixo<std::queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::priority_queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::stack<Args...>> = true;

template <typename T>
inline constexpr bool is_xixo = _is_xixo<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_exportable_object = false;

template <typename T>
inline constexpr bool is_exportable_object = _is_exportable_object<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_exportable_enum = false;

template <typename T>
inline constexpr bool is_exportable_enum = _is_exportable_enum<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_other_type = false;
template <typename... Args>
inline constexpr bool _is_other_type<std::reference_wrapper<Args...>> = true;
template <size_t N>
inline constexpr bool _is_other_type<std::bitset<N>> = true;
template <typename... Args>
inline constexpr bool _is_other_type<std::complex<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_other_type<std::variant<Args...>> = true;

template <typename T>
inline constexpr bool is_other_type = _is_other_type<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_iterable_like = is_container<T> || is_map<T> || is_set<T> ||
                                         is_tuple_like<T> || is_xixo<T> || is_exportable_object<T>;

template <typename T>
inline constexpr bool is_exportable =
    is_arithmetic<T> || is_string<T> || is_map<T> || is_set<T> || is_container<T> ||
    is_tuple_like<T> || is_xixo<T> || is_pointer<T> || is_optional<T> || is_exportable_object<T> ||
    is_exportable_enum<T> || is_other_type<T>;

}  // namespace _detail

/**
 * Check if export_var() supports type T (export_var() returns "Unsupported Type" if false).
 */
template <typename T>
inline constexpr bool is_exportable = _detail::is_exportable<T>;

}  // namespace cpp_dump
