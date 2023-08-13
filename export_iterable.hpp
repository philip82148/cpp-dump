#pragma once

#include <bits/stdc++.h>

#include "export_map.hpp"
#include "export_set.hpp"
#include "export_tuple.hpp"
#include "export_xixo.hpp"

namespace cpp_dump {

using namespace std;

template <typename T>
string export_var(T, string);

template <typename T>
constexpr auto func_iterable(T&& t) -> decltype(begin(t), end(t), true_type());
constexpr false_type func_iterable(...);

template <typename T>
constexpr bool is_func_iterable = decltype(func_iterable(declval<T>()))::value;

template <typename T>
constexpr auto member_iterable(T&& t)
    -> decltype(begin(t), end(t), true_type());
constexpr false_type member_iterable(...);

template <typename T>
constexpr bool is_member_iterable =
    decltype(member_iterable(declval<T>()))::value;

template <typename T>
constexpr bool is_iterable = (is_func_iterable<T> ||
                              is_member_iterable<T>)&&!is_map<T> &&
                             !is_set<T>;

template <typename T>
constexpr bool is_iterable_like =
    is_iterable<T> || is_map<T> || is_set<T> || is_tuple<T> || is_xixo<T>;

template <typename T, enable_if_t<is_func_iterable<T>, nullptr_t> = nullptr>
inline auto begin(T&& t) {
  using std::begin;

  return begin(std::forward<T>(t));
}

template <typename T, enable_if_t<!is_func_iterable<T> && is_member_iterable<T>,
                                  nullptr_t> = nullptr>
inline auto begin(T&& t) {
  return t.begin();
}

inline void begin(...) {}

template <typename T, enable_if_t<is_func_iterable<T>, nullptr_t> = nullptr>
inline auto end(T&& t) {
  using std::end;

  return end(std::forward<T>(t));
}

template <typename T, enable_if_t<!is_func_iterable<T> && is_member_iterable<T>,
                                  nullptr_t> = nullptr>
inline auto end(T&& t) {
  return t.begin();
}

inline void end(...) {}

template <typename T>
inline auto size(T&& t) {
  return distance(begin(t), end(t));
}

template <typename T>
using ChildType =
    remove_const_t<remove_reference_t<decltype(*begin(declval<T>()))>>;

template <typename T>
inline auto export_empty_iterable(string)
    -> enable_if_t<!is_iterable<ChildType<T>>, string> {
  return "[ ]";
}

template <typename T>
inline auto export_empty_iterable(string indent)
    -> enable_if_t<is_iterable<ChildType<T>>, string> {
  return "[ " + export_empty_iterable<ChildType<T>>(indent) + " ]";
}

template <typename T>
auto export_iterable(T value, string indent)
    -> enable_if_t<is_iterable<T> && !is_iterable_like<ChildType<T>>, string> {
  // 中身が空の時
  if (size(value) == 0) return export_empty_iterable<T>(indent);

  string content = "";

  for (auto v : value) {
    if (content != "") content += ", ";

    content += export_var(v, indent);
  }

  return "[ " + content + " ]";
}

template <typename T>
auto export_iterable(T value, string indent)
    -> enable_if_t<is_iterable_like<ChildType<T>>, string> {
  // 中身が空の時
  if (size(value) == 0) return export_empty_iterable<T>(indent);

  string content = "";

  string newIndent = indent + "  ";
  for (auto v : value) {
    if (content != "") content += ",";

    content += "\n" + newIndent + export_var(v, newIndent);
  }
  content += "\n" + indent;

  return "[" + content + "]";
}

}  // namespace cpp_dump
