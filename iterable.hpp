#pragma once

#include <bits/stdc++.h>

namespace ssk_debug {

using namespace std;

struct iterable {
 private:
  template <typename T>
  static constexpr auto Func(T&& t) -> decltype(begin(t), end(t), true_type());
  static constexpr false_type Func(...);

  template <typename T>
  static constexpr auto Member(T&& t)
      -> decltype(t.begin(), t.end(), true_type());
  static constexpr false_type Member(...);

 public:
  template <typename T>
  static constexpr bool is_iterable = (decltype(Func(declval<T>()))::value ||
                                       decltype(Member(declval<T>()))::value) &&
                                      !is_convertible_v<T, string>;

  template <typename T>
  using ChildType = decltype(*begin(declval<T>()));

  template <typename T>
  static auto begin(T&& t) {
    if constexpr (decltype(Func(declval<T>()))::value) {
      using std::begin;

      return begin(std::forward<T>(t));
    } else if constexpr (decltype(Member(declval<T>()))::value) {
      return t.begin();
    }
  }

  template <typename T>
  static auto end(T&& t) {
    if constexpr (decltype(Func(declval<T>()))::value) {
      using std::end;

      return end(std::forward<T>(t));
    } else if (decltype(Member(declval<T>()))::value) {
      return t.end();
    }
  }

  template <typename T>
  static auto size(T&& t) {
    return distance(begin(t), end(t));
  }
};

}  // namespace ssk_debug
