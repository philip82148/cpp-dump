#pragma once

#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>

namespace cpp_dump {

template <typename T>
std::string export_var(T, std::string);

template <typename T>
constexpr bool is_set = false;
template <typename T>
constexpr bool is_set<std::set<T>> = true;
template <typename T>
constexpr bool is_set<std::unordered_set<T>> = true;

template <typename T>
auto export_set(T value, std::string indent)
    -> std::enable_if_t<is_set<T>, std::string> {
  if (value.size() == 0) return "{ }";

  std::string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "{ " + content + " }";
}

}  // namespace cpp_dump
