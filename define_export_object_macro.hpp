#pragma once

#include <string>
#include <type_traits>

#include "expand_va_macro.hpp"
#include "utility.hpp"

#define CPP_DUMP_APPEND_OUTPUT_(property) append_output(#property, value.property)
#define CPP_DUMP_DEFINE_EXPORT_OBJECT(type, ...)                                                   \
  namespace cpp_dump {                                                                             \
                                                                                                   \
  extern inline int max_line_width;                                                                \
                                                                                                   \
  template <typename T>                                                                            \
  std::string export_var(T &&, std::string, size_t, bool);                                         \
                                                                                                   \
  template <typename T>                                                                            \
  auto export_object(T &&value, std::string indent, size_t last_line_length, bool fail_on_newline) \
      -> std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, type>, std::string> {             \
    std::string new_indent = indent + "  ";                                                        \
                                                                                                   \
    bool shift_indent = false;                                                                     \
                                                                                                   \
    std::string output;                                                                            \
    bool is_first;                                                                                 \
                                                                                                   \
    auto append_output = [&](std::string property_name, auto property) -> void {                   \
      if (is_first) {                                                                              \
        is_first = false;                                                                          \
      } else {                                                                                     \
        output += ", ";                                                                            \
      }                                                                                            \
                                                                                                   \
      if (shift_indent) {                                                                          \
        output += "\n" + new_indent + property_name + "= ";                                        \
        output += export_var(property, new_indent, _last_line_length(output), fail_on_newline);    \
      } else {                                                                                     \
        output += property_name + "= ";                                                            \
        output +=                                                                                  \
            export_var(property, indent, last_line_length + output.length(), fail_on_newline);     \
      }                                                                                            \
    };                                                                                             \
                                                                                                   \
  rollback:                                                                                        \
    output = #type "{ ";                                                                           \
    is_first = true;                                                                               \
                                                                                                   \
    CPP_DUMP_EXPAND_VA_(CPP_DUMP_APPEND_OUTPUT_, __VA_ARGS__);                                     \
                                                                                                   \
    if (!shift_indent) {                                                                           \
      output += " }";                                                                              \
                                                                                                   \
      if (!_has_newline(output) && last_line_length + output.length() <= max_line_width)           \
        return output;                                                                             \
                                                                                                   \
      if (fail_on_newline) return "\n";                                                            \
                                                                                                   \
      shift_indent = true;                                                                         \
      goto rollback;                                                                               \
    }                                                                                              \
                                                                                                   \
    output += "\n" + indent + "}";                                                                 \
                                                                                                   \
    return output;                                                                                 \
  }                                                                                                \
                                                                                                   \
  }  // namespace cpp_dump

#define CPP_DUMP_REGISTER_EXPORT_OBJECT()                                                        \
  namespace cpp_dump {                                                                           \
                                                                                                 \
  template <typename T>                                                                          \
  inline auto _export_var(T &&value, std::string indent, size_t last_line_length,                \
                          bool fail_on_newline) -> std::enable_if_t<is_object<T>, std::string> { \
    return export_object(value, indent, last_line_length, fail_on_newline);                      \
  }                                                                                              \
                                                                                                 \
  }  // namespace cpp_dump
