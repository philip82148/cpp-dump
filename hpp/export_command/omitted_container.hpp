/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <utility>

#include "../iterable.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename>
struct omitted_container;

template <typename T, typename It>
struct omitted_iterator {
 public:
  omitted_iterator(
      const It &it,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func,
      const std::function<std::size_t()> &original_size_func
  )
      : it(it),
        skip_size_func(skip_size_func),
        original_size_func(original_size_func),
        index(0),
        done(false) {}
  omitted_iterator() = delete;

  std::pair<bool, It> operator*() const {
    bool skip = get_skip_size().value_or(1) != 0;
    return {skip, it};
  }
  template <typename U>
  bool operator!=(const U &to) const {
    return !done && it != to.it;
  }
  omitted_iterator &operator++() {
    std::optional<std::size_t> skip_size = get_skip_size();

    if (skip_size) {
      if (skip_size == 0) {
        ++it;
        ++index;
      } else {
        iterator_advance(it, skip_size.value());
        index += skip_size.value();
      }
    } else {
      done = true;
    }

    return *this;
  }

 private:
  It it;
  const std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
      &skip_size_func;
  const std::function<std::size_t()> &original_size_func;
  std::size_t index;
  bool done;

  std::optional<std::size_t> get_skip_size() const {
    return skip_size_func(index, original_size_func);
  }
};

template <typename T>
struct omitted_container {
 public:
  omitted_container(
      const T &container,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func
  )
      : original(container),
        _begin(iterable_begin(original), skip_size_func, original_size_func),
        _end(iterable_end(original), skip_size_func, original_size_func) {}
  omitted_container() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  const T &original;
  const omitted_iterator<T, decltype(iterable_begin(original))> _begin;
  const omitted_iterator<T, decltype(iterable_end(original))> _end;

  std::optional<std::size_t> size;
  const std::function<std::size_t()> original_size_func = [this] {
    if (!size) size = iterable_size(original);
    return size.value();
  };
};

namespace skip_size_func {

inline std::optional<std::size_t> keep_front(
    std::size_t index, const std::function<std::size_t()> &, std::size_t iteration_count
) {
  if (index >= iteration_count) return std::nullopt;
  return 0;
}

inline std::optional<std::size_t> keep_back(
    std::size_t index, const std::function<std::size_t()> &get_size, std::size_t iteration_count
) {
  std::size_t size  = get_size();
  std::size_t first = size >= iteration_count ? size - iteration_count : 0;

  if (index < first) return first - index;
  return 0;
}

inline std::optional<std::size_t> keep_both_ends(
    std::size_t index, const std::function<std::size_t()> &get_size, std::size_t iteration_count
) {
  std::size_t size              = get_size();
  std::size_t first_half_last   = (iteration_count + 1) / 2;
  std::size_t rest_count        = iteration_count - first_half_last;
  std::size_t latter_half_first = size >= rest_count ? size - rest_count : 0;

  if (index >= first_half_last && index < latter_half_first) return latter_half_first - index;
  return 0;
}

inline std::optional<std::size_t> keep_middle(
    std::size_t index, const std::function<std::size_t()> &get_size, std::size_t iteration_count
) {
  std::size_t size  = get_size();
  std::size_t first = size >= iteration_count ? (size - iteration_count) / 2 : 0;
  std::size_t last  = first + iteration_count;

  if (index < first) return first - index;
  if (index >= last) return std::nullopt;
  return 0;
}

}  // namespace skip_size_func

}  // namespace _detail

}  // namespace cpp_dump
