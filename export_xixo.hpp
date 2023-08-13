#pragma once

#include <bits/stdc++.h>

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename T>
constexpr bool is_queue = false;
template <typename T>
constexpr bool is_queue<queue<T>> = true;

template <typename T>
auto export_xixo(T value, string) -> enable_if_t<is_queue<T>, string> {
  if (value.empty()) return "queue{ size= 0 }";

  return "queue{ front= " + export_var(value.front()) +
         ", size= " + to_string(value.size()) + " }";
}

template <typename T>
constexpr bool is_priority_queue = false;
template <typename T>
constexpr bool is_priority_queue<priority_queue<T>> = true;

template <typename T>
auto export_xixo(T value, string) -> enable_if_t<is_priority_queue<T>, string> {
  if (value.empty()) return "priority_queue{ size= 0 }";

  return "priority_queue{ top= " + export_var(value.top()) +
         ", size= " + to_string(value.size()) + " }";
}

template <typename T>
constexpr bool is_stack = false;
template <typename T>
constexpr bool is_stack<stack<T>> = true;

template <typename T>
auto export_xixo(T value, string) -> enable_if_t<is_stack<T>, string> {
  if (value.empty()) return "stack{ size= 0 }";

  return "stack{ top= " + export_var(value.top()) +
         ", size= " + to_string(value.size()) + " }";
}

template <typename T>
constexpr bool is_xixo = is_queue<T> || is_priority_queue<T> || is_stack<T>;

}  // namespace cpp_dump
