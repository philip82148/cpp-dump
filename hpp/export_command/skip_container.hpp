/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <optional>
#include <utility>

#include "../iterable.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename>
struct skip_container;

template <typename It>
struct skip_iterator {
 public:
  It it;

  skip_iterator(
      const It &it,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func,
      const std::function<std::size_t()> &orig_container_size
  )
      : it(it),
        skip_size_func(skip_size_func),
        orig_container_size(orig_container_size),
        index(0),
        done(false) {}
  skip_iterator() = delete;

  std::pair<bool, It> operator*() const noexcept {
    bool skip = get_skip_size().value_or(1) != 0;
    // Pass the iterator to support the case that *it is rvalue.
    return {skip, it};
  }
  template <typename It2>
  bool operator!=(const skip_iterator<It2> &to) const noexcept {
    return !done && it != to.it;
  }
  skip_iterator &operator++() noexcept {
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
  const std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
      &skip_size_func;
  const std::function<std::size_t()> &orig_container_size;
  std::size_t index;
  bool done;

  std::optional<std::size_t> get_skip_size() const noexcept {
    return skip_size_func(index, orig_container_size);
  }
};

template <typename T>
struct skip_container {
 public:
  skip_container(
      const T &container,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func
  )
      : original(container),
        _begin(iterable_begin(original), skip_size_func, orig_container_size),
        _end(iterable_end(original), skip_size_func, orig_container_size) {}
  skip_container() = delete;

  auto begin() const noexcept { return _begin; }
  auto end() const noexcept { return _end; }

 private:
  const T &original;
  const skip_iterator<decltype(iterable_begin(original))> _begin;
  const skip_iterator<decltype(iterable_end(original))> _end;

  std::optional<std::size_t> orig_container_size_cache;
  const std::function<std::size_t()> orig_container_size = [this] {
    if (!orig_container_size_cache) orig_container_size_cache = iterable_size(original);
    return orig_container_size_cache.value();
  };
};

}  // namespace _detail

}  // namespace cpp_dump
