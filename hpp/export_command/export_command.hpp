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

#include "../type_check.hpp"
#include "./skip_container.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

const std::shared_ptr<
    std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
    _default_skip_size_func_ptr(
        std::make_shared<std::function<
            std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>(
            [](std::size_t index, std::function<std::size_t()> get_size) {
              return skip_size_func::keep_front(index, get_size, max_iteration_count);
            }
        )
    );

template <typename T>
struct value_with_command;

struct export_command {
  friend export_command _map_key(export_command &&command) {
    export_command new_command(nullptr);
    new_command.map_key_child = std::make_shared<export_command>(std::move(command));
    return std::move(new_command);
  }

  friend export_command _map_value(export_command &&command) {
    export_command new_command(nullptr);
    new_command.map_value_child = std::make_shared<export_command>(std::move(command));
    return std::move(new_command);
  }

  friend export_command _map_key_and_value(export_command &&key, export_command &&value) {
    export_command new_command(nullptr);
    new_command.map_key_child   = std::make_shared<export_command>(std::move(key));
    new_command.map_value_child = std::make_shared<export_command>(std::move(value));
    return std::move(new_command);
  }

 public:
  export_command(const std::pair<int, int> &int_style)
      : int_style(int_style), skip_size_func_ptr(nullptr) {}

  export_command(
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &&skip_size_func
  )
      : int_style(0, 0),
        skip_size_func_ptr(
            skip_size_func ? std::make_shared<std::function<
                    std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>(
                std::move(skip_size_func)
            )
                           : nullptr
        ) {}

  export_command(const export_command &)            = default;
  export_command &operator=(const export_command &) = default;
  export_command(export_command &&)                 = default;
  export_command &operator=(export_command &&)      = default;

  export_command() : int_style(0, 0), skip_size_func_ptr(_default_skip_size_func_ptr) {}

  export_command &&operator<<(export_command &&command) && {
    *this << std::move(command);
    return std::move(*this);
  }

  export_command &operator<<(export_command &&command) & {
    // export_command has either {map_key_child || map_value_child}, skip_size_func_ptr
    // int_style.
    // in case of {map_key_child || map_value_child}
    if (command.map_key_child || command.map_value_child) {
      // jump to the node whose child has no skip_size_func_ptr.
      if (child && child->skip_size_func_ptr) {
        *child << std::move(command);
        return *this;
      }

      // assign to map_key_child and map_value_child of the node, which has the last
      // skip_size_func_ptr of the chain. and they inherit int_style from the node.
      auto &int_style_to_inherit = child && child->child && child->child->int_style.first
                                       ? child->child->int_style
                                       : int_style;
      if (command.map_key_child) {
        map_key_child = std::move(command.map_key_child);
        if (!map_key_child->int_style.first) map_key_child->apply_int_style(int_style_to_inherit);
      }
      if (command.map_value_child) {
        map_value_child = std::move(command.map_value_child);
        if (!map_value_child->int_style.first)
          map_value_child->apply_int_style(int_style_to_inherit);
      }
      return *this;
    }

    // in case of skip_size_func_ptr
    if (command.skip_size_func_ptr) {
      // jump to the node that has no skip_size_func_ptr
      if (skip_size_func_ptr && child) {
        *child << std::move(command);
        return *this;
      }

      // assign
      if (!skip_size_func_ptr) {
        skip_size_func_ptr = command.skip_size_func_ptr;
      } else {
        child            = std::make_shared<export_command>(std::move(command));
        child->int_style = int_style;
      }
      return *this;
    }

    // in case of int_style
    // jump to the node that has no skip_size_func_ptr
    // or create that has no skip_size_func_ptr
    if (skip_size_func_ptr) {
      if (!child) {
        child            = std::make_shared<export_command>(nullptr);
        child->int_style = int_style;
      }
      *child << std::move(command);
      return *this;
    }

    // this node is the first node that will have skip_size_func_ptr next.
    // assign int_style to the child of this node, so that int_style applies to the element of the
    // container.
    if (!child) child = std::make_shared<export_command>(nullptr);
    child->int_style = command.int_style;

    return *this;
  }

  export_command next() const {
    if (child) return *child;

    return export_command(int_style);
  }

  export_command next_for_map_key() const {
    if (skip_size_func_ptr && map_key_child) return *map_key_child;

    return next();
  }

  export_command next_for_map_value() const {
    if (skip_size_func_ptr && map_value_child) return *map_value_child;

    return next();
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) && {
    return value_with_command<T>(value, std::move(*this));
  }

  template <typename T>
  skip_container<T> get_skip_container(const T &container) const {
    if (skip_size_func_ptr) return skip_container<T>(container, *skip_size_func_ptr);
    return skip_container<T>(container, *_default_skip_size_func_ptr);
  }

  std::pair<int, int> get_int_style() const {
    auto [base, chunk] = int_style;
    return {base >= 2 && base <= 16 ? base : 0, chunk >= 0 ? chunk : 0};
  }

 private:
  std::pair<int, int> int_style;
  std::shared_ptr<
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
      skip_size_func_ptr;
  std::shared_ptr<export_command> child;
  std::shared_ptr<export_command> map_key_child;
  std::shared_ptr<export_command> map_value_child;

  void apply_int_style(const std::pair<int, int> &int_style) {
    if (!int_style.first) return;

    this->int_style = int_style;

    if (child && !child->int_style.first) child->apply_int_style(int_style);
    if (map_key_child && !map_key_child->int_style.first) map_key_child->apply_int_style(int_style);
    if (map_value_child && !map_value_child->int_style.first)
      map_value_child->apply_int_style(int_style);
  }
};

template <typename T>
struct value_with_command {
 public:
  const T &value;
  const export_command command;

  value_with_command(const T &value, export_command &&command)
      : value(value), command(std::move(command)) {}
  value_with_command() = delete;
};

template <typename>
inline constexpr bool _is_value_with_command = false;
template <typename T>
inline constexpr bool _is_value_with_command<value_with_command<T>> = true;

template <typename T>
inline constexpr bool is_value_with_command = _is_value_with_command<_remove_cref<T>>;

}  // namespace _detail

inline auto int_style(int base = 10, int chunk = 4) {
  return _detail::export_command({base, chunk});
}

inline auto keep_front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_front(index, get_size, iteration_count);
  });
}

inline auto keep_back(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_back(index, get_size, iteration_count);
  });
}

inline auto keep_both_ends(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_both_ends(index, get_size, iteration_count);
  });
}

inline auto keep_middle(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_middle(index, get_size, iteration_count);
  });
}

auto map_key(_detail::export_command &&command) { return _map_key(std::move(command)); }

auto map_value(_detail::export_command &&command) { return _map_value(std::move(command)); }

auto map_key_and_value(_detail::export_command &&key, _detail::export_command &&value) {
  return _map_key_and_value(std::move(key), std::move(value));
}

}  // namespace cpp_dump
