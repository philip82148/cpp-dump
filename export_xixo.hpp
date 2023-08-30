#pragma once

#include <string>
#include <type_traits>

#include "type_check.hpp"
#include "utility.hpp"

namespace cpp_dump {

extern inline int max_line_width;

template <typename T>
std::string export_var(const T &, std::string, size_t, bool);

template <typename T>
auto export_xixo(const T &value, std::string indent, size_t last_line_length, bool fail_on_newline)
    -> std::enable_if_t<is_queue<T>, std::string> {
  if (value.empty()) return "std::queue{ size= 0 }";

  std::string prefix = "std::queue{ front= ";
  std::string output =
      prefix +
      export_var(value.front(), indent, last_line_length + prefix.length(), fail_on_newline) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "front= ";
  output = "std::queue{\n" + prefix +
           export_var(value.front(), new_indent, prefix.length(), fail_on_newline) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
auto export_xixo(const T &value, std::string indent, size_t last_line_length, bool fail_on_newline)
    -> std::enable_if_t<is_priority_queue<T>, std::string> {
  if (value.empty()) return "std::priority_queue{ size= 0 }";

  std::string prefix = "std::priority_queue{ top= ";
  std::string output =
      prefix +
      export_var(value.top(), indent, last_line_length + prefix.length(), fail_on_newline) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::priority_queue{\n" + prefix +
           export_var(value.top(), new_indent, prefix.length(), fail_on_newline) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

template <typename T>
auto export_xixo(const T &value, std::string indent, size_t last_line_length, bool fail_on_newline)
    -> std::enable_if_t<is_stack<T>, std::string> {
  if (value.empty()) return "std::stack{ size= 0 }";

  std::string prefix = "std::stack{ top= ";
  std::string output =
      prefix +
      export_var(value.top(), indent, last_line_length + prefix.length(), fail_on_newline) +
      ", size= " + std::to_string(value.size()) + " }";

  if (!_has_newline(output) && output.length() <= max_line_width) return output;

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  prefix = new_indent + "top= ";
  output = "std::stack{\n" + prefix +
           export_var(value.top(), new_indent, prefix.length(), fail_on_newline) + ",\n" +
           new_indent + "size= " + std::to_string(value.size()) + "\n" + indent + "}";

  return output;
}

}  // namespace cpp_dump
