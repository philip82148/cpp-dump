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
      const It &it_,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func,
      const std::function<std::size_t()> &orig_container_size
  )
      : it(it_),
        _skip_size_func(skip_size_func),
        _orig_container_size(orig_container_size),
        _index(0),
        _done(false) {}
  skip_iterator(
      It &&it_,
      const std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func,
      const std::function<std::size_t()> &orig_container_size
  )
      : it(std::move(it_)),
        _skip_size_func(skip_size_func),
        _orig_container_size(orig_container_size),
        _index(0),
        _done(false) {}

  skip_iterator(skip_iterator &&) = delete;
  skip_iterator &operator=(skip_iterator &&) = delete;
  skip_iterator(const skip_iterator &) = delete;
  skip_iterator &operator=(const skip_iterator &) = delete;
  skip_iterator() = delete;

  std::pair<bool, It &> operator*() const noexcept {
    bool skip = get_skip_size().value_or(1) != 0;
    // Pass the iterator to support the case that *it is rvalue.
    // Pass the reference to support non-copyable iterators.
    // https://stackoverflow.com/questions/2568294/is-it-a-good-idea-to-create-an-stl-iterator-which-is-noncopyable
    return {skip, const_cast<It &>(it)};
  }
  template <typename It2>
  bool operator!=(const skip_iterator<It2> &to) noexcept {
    return !_done && it != to.it;
  }
  skip_iterator &operator++() noexcept {
    std::optional<std::size_t> skip_size = get_skip_size();

    if (skip_size) {
      if (skip_size == 0) {
        ++it;
        ++_index;
      } else {
        iterator_advance(it, skip_size.value());
        _index += skip_size.value();
      }
    } else {
      _done = true;
    }

    return *this;
  }

 private:
  const std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
      &_skip_size_func;
  const std::function<std::size_t()> &_orig_container_size;
  std::size_t _index;
  bool _done;

  std::optional<std::size_t> get_skip_size() const noexcept {
    return _skip_size_func(_index, _orig_container_size);
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
      : _original(container), _skip_size_func(skip_size_func) {}

  skip_container(skip_container &&) = delete;
  skip_container &operator=(skip_container &&) = delete;
  skip_container(const skip_container &) = delete;
  skip_container &operator=(const skip_container &) = delete;
  skip_container() = delete;

  auto begin() const noexcept {
    return skip_iterator(iterable_begin(_original), _skip_size_func, _orig_container_size);
  }
  auto end() const noexcept {
    return skip_iterator(iterable_end(_original), _skip_size_func, _orig_container_size);
  }

 private:
  const T &_original;
  const std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
      &_skip_size_func;

  std::optional<std::size_t> _orig_container_size_cache;
  const std::function<std::size_t()> _orig_container_size = [this] {
    if (!_orig_container_size_cache) _orig_container_size_cache = iterable_size(_original);
    return _orig_container_size_cache.value();
  };
};

}  // namespace _detail

}  // namespace cpp_dump
