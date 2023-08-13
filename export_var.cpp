#include "export_var.hpp"

#include <string>

namespace cpp_dump {

void __replace_string(std::string &, std::string, std::string);

std::string __export_var(std::string value, std::string) {
  __replace_string(value, R"(\)", R"(\\)");

  if (value.find(R"(")") == std::string::npos &&
      value.find("\n") == std::string::npos)
    return R"(")" + value + R"(")";

  __replace_string(value, R"(`)", R"(\`)");

  return "\n"
         R"(`)" +
         value + R"(`)";
}

void __replace_string(std::string &value, std::string search,
                      std::string replace) {
  std::string::size_type pos = 0;
  while ((pos = value.find(search, pos)) != std::string::npos) {
    value.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

}  // namespace cpp_dump