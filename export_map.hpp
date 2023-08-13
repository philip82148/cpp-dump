#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace cpp_dump {

template <typename T>
std::string export_var(T, std::string);

template <typename T>
constexpr bool is_map = false;
template <typename T1, typename T2>
constexpr bool is_map<std::map<T1, T2>> = true;
template <typename T1, typename T2>
constexpr bool is_map<std::unordered_map<T1, T2>> = true;

template <typename T>
auto export_map(T value, std::string indent)
    -> std::enable_if_t<is_map<T>, std::string> {
  if (value.empty()) return "{ }";

  std::string content = "";

  std::string newIndent = indent + "  ";
  for (auto pair : value) {
    if (content != "") content += ",";
    content += "\n" + newIndent + export_var(pair.first, newIndent) + ": " +
               export_var(pair.second, newIndent);
  }
  content += "\n" + indent;

  return "{" + content + "}";
}

}  // namespace cpp_dump
