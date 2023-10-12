/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "../iterable.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename T>
struct omitted_container {
 public:
  omitted_container(
      const T &container,
      const std::shared_ptr<std::function<bool(std::size_t, std::function<std::size_t()>)>>
          &is_valid
  )
      : original(container),
        is_valid(is_valid),
        _begin(*this, iterable_begin(container)),
        _end(*this, iterable_end(container)) {}
  omitted_container() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  template <typename It>
  struct omitted_iterator {
   public:
    omitted_iterator(const omitted_container<T> &parent, const It &it)
        : parent(parent), it(it), index(0) {}
    omitted_iterator() = delete;

    auto operator*() const { return std::make_pair(is_ellipsis(), *it); }
    template <typename U>
    bool operator!=(const U &to) const {
      return it != to.it;
    }
    omitted_iterator &operator++() {
      if (is_ellipsis()) {
        while (it != parent._end.it && is_ellipsis()) {
          ++it;
          ++index;
        }
      } else {
        ++it;
        ++index;
      }

      return *this;
    }

   private:
    const omitted_container<T> &parent;
    It it;
    std::size_t index;
    mutable std::optional<std::size_t> size;

    bool is_ellipsis() const {
      return !parent.is_valid->operator()(index, [this] { return original_size(); });
    }

    std::size_t original_size() const {
      if (!size) size = iterable_size(parent.original);
      return size.value();
    }
  };

  const T &original;
  const std::shared_ptr<std::function<bool(std::size_t, std::function<std::size_t()>)>> is_valid;
  const omitted_iterator<decltype(iterable_begin(original))> _begin;
  const omitted_iterator<decltype(iterable_end(original))> _end;
};

}  // namespace _detail

}  // namespace cpp_dump