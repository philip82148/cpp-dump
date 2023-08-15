#pragma once

#include <string>
#include <type_traits>

#include "type_check.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline const int max_line_width;

template <typename T>
std::string export_var(T &&, std::string);

template <typename T>
auto export_xixo(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (value.empty()) return "std::queue{ size= 0 }";

  std::string output = "std::queue{ front= ";
  output += export_var(value.front(), indent, first_line_length + output.length()) +
            ", size= " + std::to_string(value.size()) + " }";

  if (!_has_lf(output) && output.length() < max_line_width) return output;

  std::string new_indent = indent + "  ";
  output = "std::queue{\n" + new_indent + "front= ";
  output += export_var(value.front(), new_indent, first_line_length + output.length()) + ",\n" +
            new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
auto export_xixo(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (value.empty()) return "std::priority_queue{ size= 0 }";

  std::string output = "std::priority_queue{ top= ";
  output += export_var(value.top(), indent, first_line_length + output.length()) +
            ", size= " + std::to_string(value.size()) + " }";

  if (!_has_lf(output) && output.length() < max_line_width) return output;

  std::string new_indent = indent + "  ";
  output = "std::priority_queue{\n" + new_indent + "top= ";
  output += export_var(value.top(), new_indent, first_line_length + output.length()) + ",\n" +
            new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
auto export_xixo(T &&value, std::string indent, size_t first_line_length)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (value.empty()) return "std::stack{ size= 0 }";

  std::string output = "std::stack{ top= ";
  output += export_var(value.top(), indent, first_line_length + output.length()) +
            ", size= " + std::to_string(value.size()) + " }";

  std::string new_indent = indent + "  ";
  output = "std::stack{\n" + new_indent + "top= ";
  output += export_var(value.top(), new_indent, first_line_length + output.length()) + ",\n" +
            new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

}  // namespace cpp_dump
