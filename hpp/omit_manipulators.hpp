/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "./iterable.hpp"

namespace cpp_dump {

extern inline size_t max_iteration_count;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <size_t>
struct _omit_command;

template <typename T, size_t depth>
struct omitted_container {
 public:
  const T &original;
  const _omit_command<depth> &command;

  omitted_container(const T &cont, const _omit_command<depth> &command)
      : original(cont),
        command(command),
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
    omitted_iterator(const omitted_container<T, depth> &parent, It it, End end)
        : parent(parent), it(it), end(end), index(0) {}
    omitted_iterator() = delete;

    auto operator*() {
      if constexpr (depth == 0) {
        return *it;
      } else {
        return omitted_container<decltype(*it), depth - 1>(*it, *parent.command.child);
      }
    }
    auto operator->() { return it.operator->(); }
    template <typename U>
    bool operator!=(const U &to) const {
      return it != to.it;
    }
    omitted_iterator &operator++() {
      if (is_ellipsis()) {
        while (it != end && is_ellipsis()) {
          ++it;
          ++index;
        }
      } else {
        ++it;
        ++index;
      }

      return *this;
    }
    bool is_ellipsis() {
      return !parent.command.is_valid->operator()(index, [this] { return original_size(); });
    }

   private:
    const omitted_container<T, depth> &parent;
    It it;
    End end;
    size_t index;

    size_t original_size() {
      static const size_t size = iterable_size(parent.original);
      return size;
    }
  };

  omitted_iterator<decltype(iterable_begin(original)), decltype(iterable_end(original))> _begin;
  omitted_iterator<decltype(iterable_end(original)), decltype(iterable_end(original))> _end;
};

template <size_t depth>
struct _omit_command {
 public:
  const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>> is_valid;
  std::shared_ptr<_omit_command<depth - 1>> child;

  _omit_command(std::function<bool(size_t, std::function<size_t()>)> &&is_valid)
      : is_valid(std::make_shared<std::function<bool(size_t, std::function<size_t()>)>>(
          std::move(is_valid)
      )) {}
  _omit_command(const _omit_command<depth> &command)
      : is_valid(command.is_valid), child(command.child) {}
  _omit_command(const _omit_command<depth - 1> &parent, const _omit_command<0> &new_child)
      : is_valid(parent.is_valid) {
    if constexpr (depth - 1 > 0) {
      child.reset(new _omit_command<depth - 1>(parent.child, new_child));
    } else {
      child.reset(new _omit_command<0>(new_child));
    }
  }
  _omit_command() = delete;

  template <typename T>
  auto operator<<(const T &cont)
      -> std::enable_if_t<!std::is_same_v<T, _omit_command<0>>, omitted_container<T, depth>> {
    return omitted_container<T, depth>(cont, *this);
  }

  _omit_command<depth + 1> operator<<(const _omit_command<0> &command) const {
    return _omit_command<depth + 1>(*this, command);
  }
};

}  // namespace _detail

inline auto omit_front(size_t iteration_count = max_iteration_count) {
  return _detail::_omit_command<0>([=](size_t index, std::function<size_t()> get_size) -> bool {
    size_t first = get_size() - iteration_count;
    return index >= first;
  });
}

inline auto omit_middle(size_t half_iteration_count = max_iteration_count / 2) {
  return _detail::_omit_command<0>([=](size_t index, std::function<size_t()> get_size) -> bool {
    size_t latter_half_first = get_size() - half_iteration_count;
    return index < half_iteration_count || index >= latter_half_first;
  });
}

inline auto omit_back(size_t iteration_count = max_iteration_count) {
  return _detail::_omit_command<0>([=](size_t index, std::function<size_t()>) {
    return index < iteration_count;
  });
}

}  // namespace cpp_dump
