/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <vector>

namespace cpp_dump {

struct escape_sequence {
 private:
  //   struct escape_sequence_property {
  //    public:
  //     escape_sequence_property(escape_sequence &parent) : parent(parent) {}
  //     virtual void operator()(std::string_view es) = 0;
  //     std::string operator=(std::string_view es) {
  //       operator()(es);
  //       return {std::string(es)};
  //     }

  //    protected:
  //     escape_sequence &parent;
  //   };

 public:
  std::string expression;
  std::string reserved;
  std::string number;
  std::string character;
  std::string name;
  std::vector<std::string> bracket_by_depth;
  // std::string map_multiplicity;
  // std::string obj_comma;
  // std::string obj_dot;
  // std::string obj_colon;
  //   std::string map;
  //   std::string set;
  //   std::string container;
  //   std::string tuple;
  //   std::string xixo;
  //   std::string pointer;
  //   std::string optional;
  //   std::string exception;
  //   std::string other;
  //   std::string asterisk;
  //   std::string user_object;
  //   std::string user_enum;
  std::string unsupported_type;
  //   struct : public escape_sequence_property {
  //     using escape_sequence_property::escape_sequence_property,
  //     escape_sequence_property::operator=; void operator()(std::string_view es) override {
  //       parent.pointer = parent.optional = parent.other = parent.asterisk = es;
  //     }
  //   } op{*this};
  //   //   struct : public escape_sequence_property {
  //   //     using escape_sequence_property::escape_sequence_property,
  //   //     escape_sequence_property::operator=; void operator()(std::string_view es) override {
  //   //       parent.arithmetic = parent.string = parent.pointer = es;
  //   //       parent.optional = parent.other = parent.asterisk = es;
  //   //     }
  //   //   } basic{*this};
  //   struct : public escape_sequence_property {
  //     using escape_sequence_property::escape_sequence_property,
  //     escape_sequence_property::operator=; void operator()(std::string_view es) override {
  //       parent.map = parent.set = parent.container = parent.tuple = es;
  //     }
  //   } collection{*this};
  //   struct : public escape_sequence_property {
  //     using escape_sequence_property::escape_sequence_property,
  //     escape_sequence_property::operator=; void operator()(std::string_view es) override {
  //       parent.xixo = parent.exception = parent.user_object = parent.user_enum = es;
  //     }
  //   } object{*this};
};

enum class use_es { do_not_use, normal, enhanced };

namespace es_theme {

const escape_sequence simple = {
    "\e[34m", "\e[32m", "\e[36m", "\e[34m", "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "",
};

const escape_sequence rich = {
    "\e[38;2;68;193;241m",
    "\e[38;2;53;140;214m",
    "\e[38;2;167;206;155m",
    "\e[38;2;195;145;91m",
    "\e[38;2;23;159;255m",
    {"\e[38;2;255;215;0m", "\e[38;2;218;112;214m", "\e[38;2;23;159;255m"},
    // "\e[38;2;23;159;255m",
    // "\e[38;2;23;159;255m",
    // "\e[34m",
    // "\e[34m",
    // "\e[38;2;23;159;255m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "\e[34m",
    // "",
};

};  // namespace es_theme

extern inline use_es use_es_value;

extern inline escape_sequence es_value;

namespace _detail {

namespace with_es {

inline std::string _apply(const std::string &es, const std::string &s) {
  if (use_es_value == use_es::do_not_use) {
    return s;
  } else {
    const std::string reset = "\e[0m";
    return es + s + reset;
  }
}

inline std::string expression(const std::string &s) { return _apply(es_value.expression, s); }
inline std::string reserved(const std::string &s) { return _apply(es_value.reserved, s); }
inline std::string number(const std::string &s) { return _apply(es_value.number, s); }
inline std::string character(const std::string &s) { return _apply(es_value.character, s); }
inline std::string object(const std::string &s) { return _apply(es_value.name, s); }
// inline std::string op(const std::string &s) { return _apply(es_value.op, s); }

// inline std::string comma(const std::string &s) { return _apply(es_value.comma, s); }
// inline std::string dot(const std::string &s) { return _apply(es_value.dot, s); }
// inline std::string colon(const std::string &s) { return _apply(es_value.colon, s); }
// inline std::string asterisk(const std::string &s) { return _apply(es_value.asterisk, s); }

inline std::string bracket(const std::string &s, size_t d) {
  if (es_value.bracket_by_depth.size() == 0) return s;

  return _apply(es_value.bracket_by_depth[d % es_value.bracket_by_depth.size()], s);
}

}  // namespace with_es

}  // namespace _detail

}  // namespace cpp_dump