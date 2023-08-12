#pragma once

#include <bits/stdc++.h>

namespace ssk_debug {

using namespace std;

struct iterable {
 private:
  template <typename U>
  static constexpr auto Func(U&& v) -> decltype(begin(v), end(v), true_type());
  static constexpr false_type Func(...);

  template <typename U>
  static constexpr auto Member(U&& v)
      -> decltype(v.begin(), v.end(), true_type());
  static constexpr false_type Member(...);

 public:
  template <typename T>
  static constexpr bool is_iterable = (decltype(Func(declval<T>()))::value ||
                                       decltype(Member(declval<T>()))::value) &&
                                      !is_convertible_v<T, string>;

  template <typename T>
  using ChildType = decltype(*begin(declval<T>()));

  template <typename U>
  static auto begin(U&& v) {
    if constexpr (decltype(Func(declval<U>()))::value) {
      return begin(std::forward<U>(v));
    } else if constexpr (decltype(Member(declval<U>()))::value) {
      return v.begin();
    }
  }

  template <typename U>
  static auto end(U&& v) {
    if constexpr (decltype(Func(declval<U>()))::value) {
      return end(std::forward<U>(v));
    } else if (decltype(Member(declval<U>()))::value) {
      return v.end();
    }
  }
};

}  // namespace ssk_debug
