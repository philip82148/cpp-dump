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
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "./iterable.hpp"
#include "./options.hpp"
#include "./utility.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
using remove_cvref = std::remove_cv_t<std::remove_reference_t<T>>;

// It is not fully guaranteed that two or more conditions won't be true at the same time.
// For example, you can make classes that fall into both Container and Tuple categories.
// In that case, the order of comparison in the if-else-expressions within export_var() will
// determine the category of the type.

// Arithmetic -------------------------------------------------------------------------------------
template <typename T>
struct _is_vector_bool_reference {
  template <typename RawT, typename... Args>
  static auto check(int) -> std::enable_if_t<
      std::is_same_v<RawT, typename std::vector<bool, Args...>::const_reference>,
      std::true_type>;

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_vector_bool_reference = _is_vector_bool_reference<T>::value;

// inline constexpr:
// https://stackoverflow.com/questions/48041618/why-does-cppreference-define-type-traits-xxx-v-shortcuts-as-inline-constexpr-and
template <typename T>
inline constexpr bool is_arithmetic =
    std::is_arithmetic_v<remove_cvref<T>> || is_vector_bool_reference<T>;

// String -----------------------------------------------------------------------------------------
template <typename T>
inline constexpr bool is_null_pointer = std::is_null_pointer_v<remove_cvref<T>>;

template <typename T>
inline constexpr bool is_string = std::is_convertible_v<T, std::string_view> && !is_null_pointer<T>;

// Map --------------------------------------------------------------------------------------------
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
inline constexpr bool is_multimap = _is_multimap<remove_cvref<T>>;

template <typename T>
inline constexpr bool is_map = _is_map<remove_cvref<T>> || is_multimap<T>;

// Set --------------------------------------------------------------------------------------------
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
inline constexpr bool is_multiset = _is_multiset<remove_cvref<T>>;

template <typename T>
inline constexpr bool is_set = _is_set<remove_cvref<T>> || is_multiset<T>;

// Iterable ---------------------------------------------------------------------------------------
template <typename T>
struct _is_iterable {
  template <typename It>
  static auto is_incrementable(It&& u) -> decltype(++u);

  template <typename ConstT>
  static auto check(int) -> decltype(
    iterable_begin(std::declval<ConstT>()) != iterable_end(std::declval<ConstT>()),
    *iterable_begin(std::declval<ConstT>()),
    is_incrementable(iterable_begin(std::declval<ConstT>())),
    std::true_type()
    //
  );

  template <typename ConstT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<const remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_container =
    _is_iterable<T>::value && !is_string<T> && !is_map<T> && !is_set<T>;

template <typename T>
using iterable_elem_type =
    remove_cvref<decltype(*iterable_begin(std::declval<const remove_cvref<T>>()))>;

// Tuple ------------------------------------------------------------------------------------------
template <typename T>
struct _is_tuple {
  template <typename RawT>
  static auto check(int) -> decltype(std::tuple_size<RawT>::value, std::true_type());

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_tuple = _is_tuple<T>::value;

// FIFO/LIFO --------------------------------------------------------------------------------------
template <typename>
inline constexpr bool _is_xixo = false;
template <typename... Args>
inline constexpr bool _is_xixo<std::queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::priority_queue<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_xixo<std::stack<Args...>> = true;

template <typename T>
inline constexpr bool is_xixo = _is_xixo<remove_cvref<T>>;

// Pointer ----------------------------------------------------------------------------------------
template <typename>
inline constexpr bool _is_smart_pointer = false;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::unique_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::shared_ptr<Args...>> = true;
template <typename... Args>
inline constexpr bool _is_smart_pointer<std::weak_ptr<Args...>> = true;

template <typename T>
inline constexpr bool is_smart_pointer = _is_smart_pointer<remove_cvref<T>>;

template <typename T>
inline constexpr bool is_pointer =
    (std::is_pointer_v<remove_cvref<T>> && !is_string<T>) || is_null_pointer<T>
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
using remove_pointer = typename _remove_pointer<remove_cvref<T>>::type;

// Exception --------------------------------------------------------------------------------------
template <typename T>
inline constexpr bool is_exception = std::is_convertible_v<remove_cvref<T>, std::exception>;

// Other ------------------------------------------------------------------------------------------
template <typename>
inline constexpr bool _is_optional = false;
template <typename... Args>
inline constexpr bool _is_optional<std::optional<Args...>> = true;
template <>
inline constexpr bool _is_optional<std::nullopt_t> = true;

template <typename T>
inline constexpr bool is_optional = _is_optional<remove_cvref<T>>;

template <typename T>
inline constexpr bool is_type_info = std::is_convertible_v<remove_cvref<T>, std::type_index>;

template <typename>
inline constexpr bool _is_other_object = false;

template <typename T>
inline constexpr bool is_other_object = _is_other_object<remove_cvref<T>>;

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
inline constexpr bool _is_other_type<types::es_value_t> = true;

template <typename T>
inline constexpr bool is_other_type =
    is_optional<T> || is_type_info<T> || is_other_object<T> || _is_other_type<remove_cvref<T>>;

// User-defined -----------------------------------------------------------------------------------
template <typename>
inline constexpr bool _is_exportable_object = false;

template <typename T>
inline constexpr bool is_exportable_object = _is_exportable_object<remove_cvref<T>>;

// Enum -------------------------------------------------------------------------------------------
template <typename>
inline constexpr bool _is_exportable_enum = false;

template <typename T>
inline constexpr bool is_exportable_enum = _is_exportable_enum<remove_cvref<T>>;

// User-defined2 ----------------------------------------------------------------------------------
struct export_command;

template <typename T>
struct _is_exportable_object_generic {
  template <typename RawT>
  static auto check(int) -> decltype(
    export_object_generic(std::declval<RawT>(), "", 0, 0, false, std::declval<export_command>()),
    std::true_type()
    //
  );

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_exportable_object_generic = _is_exportable_object_generic<T>::value;

// Enum2 ------------------------------------------------------------------------------------------
template <typename T>
struct _is_exportable_enum_generic {
  template <typename RawT>
  static auto check(int) -> decltype(
    export_enum_generic(std::declval<RawT>(), "", 0, 0, false, std::declval<export_command>()),
    std::true_type()
    //
  );

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_exportable_enum_generic = _is_exportable_enum_generic<T>::value;

// Ostream ----------------------------------------------------------------------------------------
template <typename T>
struct _is_ostream {
  template <typename RawT>
  static auto check(int) -> std::enable_if_t<
      !std::is_function_v<RawT> && !std::is_member_pointer_v<RawT>,
      decltype(std::declval<std::ostream>() << std::declval<const RawT>(), std::true_type())>;

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_ostream = _is_ostream<T>::value;

// Asterisk ---------------------------------------------------------------------------------------
template <typename T>
struct _is_asterisk {
  template <typename RawT>
  static auto check(int) -> std::enable_if_t<
      !std::is_same_v<remove_cvref<decltype(*std::declval<const RawT>())>, RawT>,
      std::true_type>;

  template <typename RawT>
  static std::false_type check(long);

  static constexpr bool value = decltype(check<remove_cvref<T>>(0))::value;
};

template <typename T>
inline constexpr bool is_asterisk = _is_asterisk<T>::value;

// Union ------------------------------------------------------------------------------------------
template <typename T>
inline constexpr bool is_exportable =
    is_arithmetic<T> || is_string<T> || is_map<T> || is_set<T> || is_container<T> || is_tuple<T>
    || is_xixo<T> || is_pointer<T> || is_exception<T> || is_other_type<T> || is_exportable_object<T>
    || is_exportable_enum<T> || is_exportable_object_generic<T> || is_exportable_enum_generic<T>
    || is_ostream<T> || is_asterisk<T>;

template <typename T>
inline constexpr bool is_iterable_like = is_container<T> || is_map<T> || is_set<T> || is_tuple<T>;

// The return type must be a built-in type, otherwise we don't know how it will be stringified.
template <typename T>
const char* _get_typename() {
#if defined(__GNUC__) && !defined(__clang__)
  return __PRETTY_FUNCTION__;
#elif defined(__clang__)
  return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
  return __FUNCSIG__;
#else
  return "";
#endif
}

// Currently, used only by export_exception() and CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC()
template <typename T>
std::string get_typename() {
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

  std::string_view func_name = _get_typename<T>();
  std::string type_name(func_name, prefix_length, func_name.size() - prefix_length - suffix_length);

#if defined(_MSC_VER)
  type_name = replace_string(type_name, "struct ", "");
  type_name = replace_string(type_name, "class ", "");
  type_name = replace_string(type_name, "enum ", "");
#endif

  return type_name;
}

}  // namespace _detail

}  // namespace cpp_dump
