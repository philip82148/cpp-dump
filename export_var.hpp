#pragma once

#include <string>
#include <type_traits>

namespace cpp_dump {

template <typename T>
std::string export_var(T &&, std::string = "");

}  // namespace cpp_dump

#include "export_container.hpp"
#include "export_map.hpp"
#include "export_set.hpp"
#include "export_tuple_like.hpp"
#include "export_xixo.hpp"
#include "iterable_like.hpp"

namespace cpp_dump {

using namespace std;

inline std::string __export_var(bool value, std::string) {
  return value ? "true" : "false";
}

template <typename T>
inline auto __export_var(T &&value, std::string)
    -> decltype(std::to_string(value)) {
  return std::to_string(value);
}

std::string __export_var(std::string, std::string);

inline std::string __export_var(const char *value, std::string indent) {
  return __export_var((std::string)(value), indent);
}

inline std::string __export_var(char value, std::string) {
  return "'" + std::string{value} + "'";
}

template <typename T>
inline auto __export_var(T &&value, std::string indent)
    -> std::enable_if_t<is_set<T>, std::string> {
  return export_set(value, indent);
}

template <typename T>
inline auto __export_var(T &&value, std::string indent)
    -> std::enable_if_t<is_map<T>, std::string> {
  return export_map(value, indent);
}

template <typename T>
inline auto __export_var(T &&value, std::string indent)
    -> std::enable_if_t<is_tuple_like<T>, std::string> {
  return export_tuple_like(value, indent);
}

template <typename T>
inline auto __export_var(T &&value, std::string indent)
    -> std::enable_if_t<is_container<T>, std::string> {
  return export_container(value, indent);
}

template <typename T>
inline auto __export_var(T &&value, std::string indent)
    -> std::enable_if_t<is_xixo<T>, std::string> {
  return export_xixo(value, indent);
}

template <typename T>
inline std::string export_var(T &&value, std::string indent) {
  return __export_var(value, indent);
}

}  // namespace cpp_dump
