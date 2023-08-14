#pragma once

#include <string>

namespace cpp_dump {

inline bool __has_lf(std::string value) {
  return value.find("\n") != std::string::npos;
}

inline size_t __first_line_length(std::string value) {
  auto lf_pos = value.find("\n");

  if (lf_pos == std::string::npos) return value.length();

  return lf_pos;
}

inline size_t __last_line_length(std::string value,
                                 int additional_first_line_length = 0) {
  auto lf_pos = value.rfind("\n");

  if (lf_pos == std::string::npos)
    return additional_first_line_length + value.length();

  return value.length() - lf_pos - 1;
}

}  // namespace cpp_dump