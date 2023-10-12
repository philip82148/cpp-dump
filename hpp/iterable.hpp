/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <iterator>

namespace cpp_dump {

namespace _detail {

template <typename T>
auto _iterable_begin(const T &t, int) -> decltype(begin(t)) {
  return begin(t);
}

template <typename T>
auto _iterable_begin(const T &t, long) -> decltype(std::begin(t)) {
  return std::begin(t);
}

template <typename T>
auto iterable_begin(const T &t) -> decltype(_iterable_begin(t, 0)) {
  return _iterable_begin(t, 0);
}

template <typename T>
auto _iterable_end(const T &t, int) -> decltype(end(t)) {
  return end(t);
}

template <typename T>
auto _iterable_end(const T &t, long) -> decltype(std::end(t)) {
  return std::end(t);
}

template <typename T>
auto iterable_end(const T &t) -> decltype(_iterable_end(t, 0)) {
  return _iterable_end(t, 0);
}

template <typename T>
inline bool is_empty_iterable(const T &t) {
  return !(iterable_begin(t) != iterable_end(t));
}

template <typename T>
inline auto _iterable_size(const T &t, int) -> decltype(std::size(t)) {
  return std::size(t);
}

template <typename T>
inline auto _iterable_size(const T &t, long)
    -> decltype(std::distance(iterable_begin(t), iterable_end(t))) {
  return std::distance(iterable_begin(t), iterable_end(t));
}

template <typename T>
inline std::size_t _iterable_size(const T &t, ...) {
  std::size_t size = 0;
  for (const auto &_e : t) (void)_e, ++size;
  return size;
}

template <typename T>
inline std::size_t iterable_size(const T &t) {
  return _iterable_size(t, 0);
}

}  // namespace _detail

}  // namespace cpp_dump
