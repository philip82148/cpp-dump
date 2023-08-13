#pragma once

#include <queue>
#include <stack>
#include <string>
#include <type_traits>

namespace cpp_dump {

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
constexpr bool __is_queue = false;
template <typename T>
constexpr bool __is_queue<std::queue<T>> = true;

template <typename T>
constexpr bool is_queue = __is_queue<std::remove_reference_t<T>>;

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (value.empty()) return "std::queue{ size= 0 }";

  return "std::queue{ front= " + export_var(value.front()) +
         ", size= " + std::to_string(value.size()) + " }";
}

template <typename T>
constexpr bool __is_priority_queue = false;
template <typename T>
constexpr bool __is_priority_queue<std::priority_queue<T>> = true;

template <typename T>
constexpr bool is_priority_queue =
    __is_priority_queue<std::remove_reference_t<T>>;

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (value.empty()) return "std::priority_queue{ size= 0 }";

  return "std::priority_queue{ top= " + export_var(value.top()) +
         ", size= " + std::to_string(value.size()) + " }";
}

template <typename T>
constexpr bool __is_stack = false;
template <typename T>
constexpr bool __is_stack<std::stack<T>> = true;

template <typename T>
constexpr bool is_stack = __is_stack<std::remove_reference_t<T>>;

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (value.empty()) return "std::stack{ size= 0 }";

  return "std::stack{ top= " + export_var(value.top()) +
         ", size= " + std::to_string(value.size()) + " }";
}

template <typename T>
constexpr bool is_xixo = is_queue<T> || is_priority_queue<T> || is_stack<T>;

}  // namespace cpp_dump
