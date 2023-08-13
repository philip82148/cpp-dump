#pragma once

#include <string>
#include <tuple>

namespace cpp_dump {

template <typename T>
std::string export_var(T, std::string);

template <typename... Args>
constexpr bool is_tuple = false;
template <typename T1, typename T2>
constexpr bool is_tuple<std::pair<T1, T2>> = true;
template <typename... Args>
constexpr bool is_tuple<std::tuple<Args...>> = true;

template <const int i, const int size, typename... Args>
std::string __export_tuple(std::tuple<Args...> value, std::string indent) {
  if constexpr (i < size - 1) {
    return export_var(std::get<i>(value), indent) + ", " +
           __export_tuple<i + 1, size>(value, indent);
  } else {
    return export_var(std::get<i>(value), indent);
  }
}

template <typename... Args>
std::string export_tuple(std::tuple<Args...> value, std::string indent) {
  return "( " +
         __export_tuple<0, std::tuple_size_v<std::tuple<Args...>>>(value,
                                                                   indent) +
         " )";
}

template <typename T1, typename T2>
inline std::string export_tuple(std::pair<T1, T2> value, std::string indent) {
  return export_tuple(std::make_tuple(value.first, value.second), indent);
}

}  // namespace cpp_dump
