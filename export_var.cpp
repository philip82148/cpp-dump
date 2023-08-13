#include "export_var.hpp"

#include <bits/stdc++.h>

namespace cpp_dump {

using namespace std;

void replaceAll(string&, string, string);

string export_var_real(string value, string) {
  replaceAll(value, R"(\)", R"(\\)");

  if (value.find(R"(")") == string::npos && value.find("\n") == string::npos)
    return R"(")" + value + R"(")";

  replaceAll(value, R"(`)", R"(\`)");

  return "\n"
         R"(`)" +
         value + R"(`)";
}

void replaceAll(string& value, string search, string replace) {
  string::size_type pos = 0;
  while ((pos = value.find(search, pos)) != string::npos) {
    value.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

}  // namespace cpp_dump