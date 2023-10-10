/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <string>

#include "./iterable.hpp"

namespace cpp_dump {

extern inline size_t max_iteration_count;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
struct omitted_container {
 public:
  const T &original;
  const std::function<bool(size_t)> is_valid;

  omitted_container(const T &cont, std::function<bool(size_t)> is_valid)
      : original(cont),
        is_valid(is_valid),
        _begin(*this, iterable_begin(cont), iterable_end(cont)),
        _end(*this, iterable_end(cont), iterable_end(cont)) {}
  omitted_container() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }
  auto empty() const { return !(_begin != _end); }

 private:
  template <typename It, typename End>
  struct omitted_iterator {
   public:
    omitted_iterator(const omitted_container<T> &parent, It it, End end)
        : parent(parent), it(it), end(end), index(0) {}
    omitted_iterator() = delete;

    auto operator*() { return *it; }
    auto operator->() { return it.operator->(); }
    template <typename U>
    bool operator!=(const U &to) const {
      return it != to.it;
    }
    omitted_iterator &operator++() {
      if (parent.is_valid(index)) {
        ++it;
        ++index;
      } else {
        while (it != end && !parent.is_valid(index)) {
          ++it;
          ++index;
        }
      }

      return *this;
    }
    bool is_ellipsis() { return !parent.is_valid(index); }

   private:
    const omitted_container<T> &parent;
    It it;
    End end;
    size_t index;
  };

  omitted_iterator<decltype(iterable_begin(original)), decltype(iterable_end(original))> _begin;
  omitted_iterator<decltype(iterable_end(original)), decltype(iterable_end(original))> _end;
};

struct _omit_front {
 public:
  _omit_front(size_t iteration_count) : iteration_count(iteration_count) {}
  _omit_front() = delete;

  template <typename T>
  omitted_container<T> operator<<(const T &cont) {
    size_t first = iterable_size(cont) - iteration_count;
    return omitted_container<T>(cont, [=, this](size_t index) { return index >= first; });
  }

 private:
  size_t iteration_count;
};

struct _omit_middle {
 public:
  _omit_middle(size_t half_iteration_count) : half_iteration_count(half_iteration_count) {}
  _omit_middle() = delete;

  template <typename T>
  omitted_container<T> operator<<(const T &cont) {
    size_t latter_half_first = iterable_size(cont) - half_iteration_count;
    return omitted_container<T>(cont, [=, this](size_t index) {
      return index < half_iteration_count || index >= latter_half_first;
    });
  }

 private:
  size_t half_iteration_count;
};

struct _omit_back {
 public:
  _omit_back(size_t iteration_count) : iteration_count(iteration_count) {}
  _omit_back() = delete;

  template <typename T>
  omitted_container<T> operator<<(const T &cont) {
    return omitted_container<T>(cont, [this](size_t index) { return index < iteration_count; });
  }

 private:
  size_t iteration_count;
};

}  // namespace _detail

inline auto omit_front(size_t iteration_count = max_iteration_count) {
  return _detail::_omit_front(iteration_count);
}

template <typename T>
inline auto omit_front(T t, size_t iteration_count = max_iteration_count) {
  return omit_front(iteration_count) << t;
}

inline auto omit_middle(size_t half_iteration_count = max_iteration_count / 2) {
  return _detail::_omit_middle(half_iteration_count);
}

template <typename T>
inline auto omit_middle(T t, size_t iteration_count = max_iteration_count) {
  return omit_middle(iteration_count) << t;
}

inline auto omit_back(size_t iteration_count = max_iteration_count) {
  return _detail::_omit_back(iteration_count);
}

template <typename T>
inline auto omit_back(T t, size_t iteration_count = max_iteration_count) {
  return omit_back(iteration_count) << t;
}

}  // namespace cpp_dump
