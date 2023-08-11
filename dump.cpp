#include "dump.hpp"

#include <string>

namespace ssk_debug {

using namespace std;

void replaceAll(string&, string, string);

template <>
string export_var(bool value) {
  return value ? "true" : "false";
}

template <>
string export_var(const char* value) {
  return export_var((string)value);
}

template <>
string export_var(string value) {
  if (value.find('\n') == string::npos) {
    replaceAll(value, R"(\)", R"(\\)");
    replaceAll(value, R"(")", R"(\")");

    return '"' + value + '"';
  }

  return R"(""")"
         "\n" +
         value +
         "\n"
         R"(""")";
}

void replaceAll(string& value, string search, string replace) {
  string::size_type pos = 0;
  while ((pos = value.find(search, pos)) != string::npos) {
    value.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

}  // namespace ssk_debug