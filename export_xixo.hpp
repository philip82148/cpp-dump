#pragma once

#include <string>
#include <type_traits>

#include "iterable_like.hpp"

namespace cpp_dump {

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (value.empty()) return "std::queue{ size= 0 }";

  return "std::queue{ front= " + export_var(value.front()) +
         ", size= " + std::to_string(value.size()) + " }";
}

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (value.empty()) return "std::priority_queue{ size= 0 }";

  return "std::priority_queue{ top= " + export_var(value.top()) +
         ", size= " + std::to_string(value.size()) + " }";
}

template <typename T>
auto export_xixo(T &&value, std::string)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (value.empty()) return "std::stack{ size= 0 }";

  return "std::stack{ top= " + export_var(value.top()) +
         ", size= " + std::to_string(value.size()) + " }";
}

}  // namespace cpp_dump
