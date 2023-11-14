#include <string>

#include "../dump.hpp"

enum class enum_a { s, k, l };

struct class_a {
  int int_a = 5;
  std::string a_str() const { return std::to_string(int_a); }
};

struct class_b {
  int int_b;
  std::string b_str() const { return std::to_string(int_b); }
};

CPP_DUMP_DEFINE_EXPORT_ENUM(enum_a, enum_a::s, enum_a::k);
CPP_DUMP_DEFINE_EXPORT_OBJECT(class_a, int_a, a_str());
CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT(int_b, b_str());

void odr_test();
