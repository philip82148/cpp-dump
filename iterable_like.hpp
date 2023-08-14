#pragma once

#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace cpp_dump {

template <typename T>
inline constexpr auto __is_iterable(T &&t)
    -> decltype(std::begin(t), std::end(t), std::true_type());
inline constexpr std::false_type __is_iterable(...);

template <typename T>
inline constexpr bool is_iterable =
    decltype(__is_iterable(std::declval<T>()))::value;

template <typename T>
using iterable_elem_type = std::remove_const_t<
    std::remove_reference_t<decltype(*std::begin(std::declval<T>()))>>;

template <typename T>
inline bool is_empty_iterable(T &&t) {
  return std::distance(std::begin(t), std::end(t)) == 0;
}

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<T, std::string>;

template <typename T>
inline constexpr bool __is_map = false;
template <typename T1, typename T2>
inline constexpr bool __is_map<std::map<T1, T2>> = true;
template <typename T1, typename T2>
inline constexpr bool __is_map<std::unordered_map<T1, T2>> = true;

template <typename T>
inline constexpr bool is_map = __is_map<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool __is_set = false;
template <typename T>
inline constexpr bool __is_set<std::set<T>> = true;
template <typename T>
inline constexpr bool __is_set<std::unordered_set<T>> = true;

template <typename T>
inline constexpr bool is_set = __is_set<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool is_container =
    is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename... Args>
inline constexpr bool __is_tuple = false;
template <typename... Args>
inline constexpr bool __is_tuple<std::tuple<Args...>> = true;

template <typename T>
inline constexpr bool is_tuple = __is_tuple<std::remove_reference_t<T>>;

template <typename... Args>
inline constexpr bool __is_pair = false;
template <typename T1, typename T2>
inline constexpr bool __is_pair<std::pair<T1, T2>> = true;

template <typename T>
inline constexpr bool is_pair = __is_pair<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool is_tuple_like = is_tuple<T> || is_pair<T>;

template <typename T>
inline constexpr bool __is_queue = false;
template <typename T>
inline constexpr bool __is_queue<std::queue<T>> = true;

template <typename T>
inline constexpr bool is_queue = __is_queue<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool __is_priority_queue = false;
template <typename T>
inline constexpr bool __is_priority_queue<std::priority_queue<T>> = true;

template <typename T>
inline constexpr bool is_priority_queue =
    __is_priority_queue<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool __is_stack = false;
template <typename T>
inline constexpr bool __is_stack<std::stack<T>> = true;

template <typename T>
inline constexpr bool is_stack = __is_stack<std::remove_reference_t<T>>;

template <typename T>
inline constexpr bool is_xixo =
    is_queue<T> || is_priority_queue<T> || is_stack<T>;

template <typename T>
inline constexpr bool is_iterable_like =
    is_iterable<T> || is_tuple_like<T> || is_xixo<T>;

}  // namespace cpp_dump