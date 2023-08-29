#pragma once

#include <string>
#include <type_traits>

namespace cpp_dump {

// template <typename T>
// std::string export_var(T &&, std::string = "", size_t = 0, bool = false);

}  // namespace cpp_dump

#include "export_container.hpp"
#include "export_map.hpp"
#include "export_set.hpp"
#include "export_string.hpp"
#include "export_tuple_like.hpp"
#include "export_xixo.hpp"
#include "type_check.hpp"

namespace cpp_dump {

inline std::string _export_var(bool value, std::string, size_t, bool) {
  return value ? "true" : "false";
}

template <typename T>
inline auto _export_var(T &&value, std::string, size_t, bool) -> decltype(std::to_string(value)) {
  return std::to_string(value);
}

inline std::string _export_var(char value, std::string, size_t, bool) {
  return "'" + std::string{value} + "'";
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_string<T>, std::string> {
  return export_string(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_set<T>, std::string> {
  return export_set(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_map<T>, std::string> {
  return export_map(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_tuple_like<T>, std::string> {
  return export_tuple_like(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_container<T>, std::string> {
  return export_container(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline auto _export_var(T &&value, std::string indent, size_t last_line_length,
                        bool fail_on_newline) -> std::enable_if_t<is_xixo<T>, std::string> {
  return export_xixo(value, indent, last_line_length, fail_on_newline);
}

template <typename T>
inline std::string export_var(T &&value, std::string indent, size_t last_line_length,
                              bool fail_on_newline) {
  return _export_var(value, indent, last_line_length, fail_on_newline);
}

}  // namespace cpp_dump
