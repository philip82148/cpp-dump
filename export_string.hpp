#pragma once

#include <string>

#include "utility.hpp"

namespace cpp_dump {

inline void _replace_string(std::string &value, const std::string &search,
                            const std::string &replace) {
  std::string::size_type pos = 0;
  while ((pos = value.find(search, pos)) != std::string::npos) {
    value.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

inline std::string export_string(std::string value, const std::string &, size_t,
                                 bool fail_on_newline) {
  _replace_string(value, R"(\)", R"(\\)");

  if (!_has_newline(value) && value.find(R"(")") == std::string::npos)
    return R"(")" + value + R"(")";

  _replace_string(value, R"(`)", R"(\`)");

  if (!_has_newline(value)) return R"(`)" + value + R"(`)";

  if (fail_on_newline) return "\n";

  return "\n"
         R"(`)" +
         value + R"(`)";
}

}  // namespace cpp_dump