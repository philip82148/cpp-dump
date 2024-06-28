/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <bitset>
#include <complex>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <variant>

#include "./iterable.hpp"
#include "./options.hpp"
#include "./utility.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
using _remove_cref = std::remove_const_t<std::remove_reference_t<T>>;

template <typename T>
auto _incrementable(T&& t) -> decltype(++t);

template <typename T>
auto _is_iterable(int) -> decltype(
  iterable_begin(std::declval<T>()) != iterable_end(std::declval<T>()),
  *iterable_begin(std::declval<T>()),
  _incrementable(iterable_begin(std::declval<T>())),
  std::true_type()
  //
);

template <typename>
std::false_type _is_iterable(long);

// inline constexpr:
// https://stackoverflow.com/questions/48041618/why-does-cppreference-define-type-traits-xxx-v-shortcuts-as-inline-constexpr-and
template <typename T>
inline constexpr bool is_iterable = decltype(_is_iterable<const _remove_cref<T>>(0))::value;

template <typename T>
using iterable_elem_type =
    _remove_cref<decltype(*iterable_begin(std::declval<const _remove_cref<T>>()))>;

template <typename T>
inline constexpr bool is_arithmetic = std::is_arithmetic_v<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_null_pointer = std::is_null_pointer_v<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<T, std::string_view> && !is_null_pointer<T>;

template <typename>
inline constexpr bool _is_map = false;
template <typename... Args>
inline constexpr bool _is_map<std::map<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_map<std::unordered_map<Args...>> = true;

template <typename>
inline constexpr bool _is_multimap = false;
template <typename... Args>
inline constexpr bool _is_multimap<std::multimap<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_multimap<std::unordered_multimap<Args...>> = true;

template <typename T>
inline constexpr bool is_multimap = _is_multimap<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_map = _is_map<_remove_cref<T>> || is_multimap<T>;

template <typename>
inline constexpr bool _is_set = false;
template <typename... Args>
inline constexpr bool _is_set<std::set<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_set<std::unordered_set<Args...>> = true;

template <typename>
inline constexpr bool _is_multiset = false;
template <typename... Args>
inline constexpr bool _is_multiset<std::multiset<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_multiset<std::unordered_multiset<Args...>> = true;

template <typename T>
inline constexpr bool is_multiset = _is_multiset<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_set = _is_set<_remove_cref<T>> || is_multiset<T>;

template <typename T>
inline constexpr bool is_container = is_iterable<T> && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename T>
auto _is_tuple(int) -> decltype(std::tuple_size<T>::value, std::true_type());
template <typename>
std::false_type _is_tuple(long);

template <typename T>
inline constexpr bool is_tuple = decltype(_is_tuple<_remove_cref<T>>(0))::value;

template <typename>
inline constexpr bool _is_xixo = false;
template <typename... Args>
inline constexpr bool _is_xixo<std::queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::priority_queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::stack<Args...>> = true;

template <typename T>
inline constexpr bool is_xixo = _is_xixo<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_smart_pointer = false;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::unique_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::shared_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::weak_ptr<Args...>> = true;

template <typename T>
inline constexpr bool is_smart_pointer = _is_smart_pointer<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_pointer =
    (std::is_pointer_v<_remove_cref<T>> && !is_string<T>) || is_null_pointer<T>
    || is_smart_pointer<T>;

template <typename T, typename = void>
struct _remove_pointer {
  using type = std::remove_pointer_t<T>;
};
template <typename T>
struct _remove_pointer<T, std::enable_if_t<is_smart_pointer<T>>> {
  using type = typename T::element_type;
};

template <typename T>
using remove_pointer = typename _remove_pointer<_remove_cref<T>>::type;

template <typename T>
inline constexpr bool is_exception = std::is_convertible_v<_remove_cref<T>, std::exception>;

template <typename>
inline constexpr bool _is_optional = false;
template <typename... Args>
inline constexpr bool _is_optional<std::optional<Args...>> = true;
template <>
inline constexpr bool _is_optional<std::nullopt_t> = true;

template <typename T>
inline constexpr bool is_optional = _is_optional<_remove_cref<T>>;

template <typename T>
inline constexpr bool is_type_info = std::is_convertible_v<_remove_cref<T>, std::type_index>;

template <typename>
inline constexpr bool _is_other_object = false;

template <typename T>
inline constexpr bool is_other_object = _is_other_object<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_other_type = false;
template <typename... Args>
inline constexpr bool _is_other_type<std::reference_wrapper<Args...>> = true;
template <std::size_t N>
inline constexpr bool _is_other_type<std::bitset<N>> = true;
template <typename... Args>
inline constexpr bool _is_other_type<std::complex<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_other_type<std::variant<Args...>> = true;
template <>
inline constexpr bool _is_other_type<es_value_t> = true;

template <typename T>
inline constexpr bool is_other_type =
    is_optional<T> || is_type_info<T> || is_other_object<T> || _is_other_type<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_exportable_object = false;

template <typename T>
inline constexpr bool is_exportable_object = _is_exportable_object<_remove_cref<T>>;

template <typename>
inline constexpr bool _is_exportable_enum = false;

template <typename T>
inline constexpr bool is_exportable_enum = _is_exportable_enum<_remove_cref<T>>;

template <typename T>
inline constexpr bool _is_exportable_partial =
    is_arithmetic<T> || is_string<T> || is_map<T> || is_set<T> || is_container<T> || is_tuple<T>
    || is_xixo<T> || is_pointer<T> || is_exception<T> || is_other_type<T> || is_exportable_object<T>
    || is_exportable_enum<T>;

template <typename T>
auto _is_ostream(int) -> std::enable_if_t<
    !_is_exportable_partial<T> && !std::is_function_v<T> && !std::is_member_pointer_v<T>,
    decltype(std::declval<std::ostream>() << std::declval<const T>(), std::true_type())>;
template <typename>
std::false_type _is_ostream(long);

template <typename T>
inline constexpr bool is_ostream = decltype(_is_ostream<_remove_cref<T>>(0))::value;

struct export_command;

template <typename T>
auto _is_dangerously_exportable_object(int) -> std::enable_if_t<
    !_is_exportable_partial<T> && !is_ostream<T>,
    decltype(dangerous_export_object(std::declval<T>(), "", 0, 0, false, std::declval<export_command>()), std::true_type())>;
template <typename>
std::false_type _is_dangerously_exportable_object(long);

template <typename T>
inline constexpr bool is_dangerously_exportable_object =
    decltype(_is_dangerously_exportable_object<_remove_cref<T>>(0))::value;

template <typename T>
auto _is_asterisk(int) -> std::enable_if_t<
    !_is_exportable_partial<T> && !is_ostream<T> && !is_dangerously_exportable_object<T>
        && !std::is_same_v<_remove_cref<decltype(*std::declval<const T>())>, T>,
    std::true_type>;
template <typename>
std::false_type _is_asterisk(long);

template <typename T>
inline constexpr bool is_asterisk = decltype(_is_asterisk<_remove_cref<T>>(0))::value;

template <typename T>
inline constexpr bool is_exportable = _is_exportable_partial<T> || is_ostream<T>
                                      || is_dangerously_exportable_object<T> || is_asterisk<T>;

template <typename T>
inline constexpr bool is_iterable_like = is_container<T> || is_map<T> || is_set<T> || is_tuple<T>;

inline std::string _get_typename_aux(std::string func_name) {
#if defined(__GNUC__) && !defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char* cpp_dump::_detail::_get_typename() [with T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(__clang__)
  constexpr std::size_t prefix_length =
      std::string_view("const char *cpp_dump::_detail::_get_typename() [T = ").size();
  constexpr std::size_t suffix_length = std::string_view("]").size();
#elif defined(_MSC_VER)
  constexpr std::size_t prefix_length =
      std::string_view("const char *__cdecl cpp_dump::_detail::_get_typename<").size();
  constexpr std::size_t suffix_length = std::string_view(">(void)").size();
#else
  constexpr std::size_t prefix_length = 0;
  constexpr std::size_t suffix_length = 0;
#endif

  std::string type_name =
      func_name.substr(prefix_length, func_name.length() - prefix_length - suffix_length);

#if defined(_MSC_VER)
  replace_string(type_name, "struct ", "");
  replace_string(type_name, "class ", "");
#endif

  return type_name;
}

// The return type must be a built-in type, otherwise we don't know how it will be stringified.
template <typename T>
const char* _get_typename() {
  static std::string type_name = _get_typename_aux(
#if defined(__GNUC__) && !defined(__clang__)
      __PRETTY_FUNCTION__
#elif defined(__clang__)
      __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
      __FUNCSIG__
#else
      ""
#endif
  );

  return type_name.c_str();
}

// Currently, used only by export_exception() and CPP_DUMP_DEFINE_DANGEROUS_EXPORT_OBJECT()
template <typename T>
std::string get_typename() {
  return _get_typename<T>();
}

}  // namespace _detail

}  // namespace cpp_dump
