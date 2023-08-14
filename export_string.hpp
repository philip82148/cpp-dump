#pragma once

#include <string>

namespace cpp_dump {

inline void _replace_string(std::string &value, std::string search, std::string replace) {
  std::string::size_type pos = 0;
  while ((pos = value.find(search, pos)) != std::string::npos) {
    value.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

inline std::string export_string(std::string value) {
  _replace_string(value, R"(\)", R"(\\)");

  if (value.find(R"(")") == std::string::npos && value.find("\n") == std::string::npos)
    return R"(")" + value + R"(")";

  _replace_string(value, R"(`)", R"(\`)");

  return "\n"
         R"(`)" +
         value + R"(`)";
}

}  // namespace cpp_dump