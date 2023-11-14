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
#include <tuple>
#include <utility>

#include "../options.hpp"
#include "../type_check.hpp"
#include "./skip_container.hpp"

namespace cpp_dump {

namespace _detail {

const std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
    _default_skip_size_func(
        [](std::size_t index, const std::function<std::size_t()> &) -> std::optional<std::size_t> {
          if (index >= max_iteration_count) return std::nullopt;
          return 0;
        }
    );

template <typename T>
struct value_with_command;

struct export_command {
  friend export_command _map_key(export_command &&command) {
    export_command new_command(nullptr);
    new_command._map_key_child = std::make_unique<export_command>(std::move(command));
    return new_command;
  }

  friend export_command _map_value(export_command &&command) {
    export_command new_command(nullptr);
    new_command._map_value_child = std::make_unique<export_command>(std::move(command));
    return new_command;
  }

  friend export_command _map_key_and_value(export_command &&key, export_command &&value) {
    export_command new_command(nullptr);
    new_command._map_key_child = std::make_unique<export_command>(std::move(key));
    new_command._map_value_child = std::make_unique<export_command>(std::move(value));
    return new_command;
  }

 public:
  static const export_command default_command;

  export_command(const std::tuple<unsigned int, unsigned int, unsigned int, bool, bool> &int_style
  ) {
    auto base = std::get<0>(int_style);
    if (base >= 2 && base <= 16) _int_style = int_style;
  }

  export_command(
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &&skip_size_func
  )
      : _skip_size_func(std::move(skip_size_func)) {}

  export_command(const std::function<
                 std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
                     &skip_size_func)
      : _skip_size_func(skip_size_func) {}

  export_command(export_command &&) = default;
  export_command &operator=(export_command &&) = default;
  export_command(const export_command &) = delete;
  export_command &operator=(const export_command &) = delete;
  export_command() = delete;

  export_command &&operator<<(export_command &&command) && {
    *this << std::move(command);
    return std::move(*this);
  }

  export_command &operator<<(export_command &&command) & {
    // export_command has either {map_key_child || map_value_child}, skip_size_func
    // int_style.
    // in case of {map_key_child || map_value_child}
    if (command._map_key_child || command._map_value_child) {
      // jump to the node whose child has no skip_size_func.
      if (_child && _child->_skip_size_func) {
        *_child << std::move(command);
        return *this;
      }

      // assign to map_key_child and map_value_child of the node, which has the last
      // skip_size_func of the chain. and they inherit int_style from the node.
      const auto &int_style_to_inherit = _child && _child->_child && _child->_child->_int_style
                                             ? _child->_child->_int_style
                                             : _int_style;
      if (command._map_key_child) {
        _map_key_child = std::move(command._map_key_child);
        if (int_style_to_inherit && !_map_key_child->_int_style)
          _map_key_child->apply_int_style(int_style_to_inherit);
      }
      if (command._map_value_child) {
        _map_value_child = std::move(command._map_value_child);
        if (int_style_to_inherit && !_map_value_child->_int_style)
          _map_value_child->apply_int_style(int_style_to_inherit);
      }
      return *this;
    }

    // in case of skip_size_func
    if (command._skip_size_func) {
      // jump to the node that has no skip_size_func
      if (_skip_size_func && _child) {
        *_child << std::move(command);
        return *this;
      }

      // assign
      if (!_skip_size_func) {
        _skip_size_func = command._skip_size_func;
      } else {
        _child = std::make_unique<export_command>(std::move(command));
        _child->_int_style = _int_style;
      }
      return *this;
    }

    // in case of int_style
    // jump to the node that has no skip_size_func
    // or create that has no skip_size_func
    if (_skip_size_func) {
      if (!_child) {
        _child = std::make_unique<export_command>(nullptr);
        _child->_int_style = _int_style;
      }
      *_child << std::move(command);
      return *this;
    }

    // this node is the first node that will have skip_size_func next.
    // assign int_style to the child of this node, so that int_style applies to the element of the
    // container.
    if (!_child) _child = std::make_unique<export_command>(nullptr);
    _child->_int_style = command._int_style;

    return *this;
  }

  const export_command &next() const {
    if (!_child) {
      if (!_int_style) return default_command;

      _child = std::make_unique<export_command>(_int_style.value());
    }

    return *_child;
  }

  const export_command &next_for_map_key() const {
    if (_skip_size_func && _map_key_child) return *_map_key_child;

    return next();
  }

  const export_command &next_for_map_value() const {
    if (_skip_size_func && _map_value_child) return *_map_value_child;

    return next();
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) && {
    return value_with_command<T>(value, std::move(*this));
  }

  template <typename T>
  skip_container<T> create_skip_container(const T &container) const {
    if (_skip_size_func) return skip_container<T>(container, _skip_size_func);
    return skip_container<T>(container, _default_skip_size_func);
  }

  std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>> get_int_style(
  ) const {
    return _int_style;
  }

 private:
  std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>> _int_style;
  std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
      _skip_size_func;
  mutable std::unique_ptr<export_command> _child;
  std::unique_ptr<export_command> _map_key_child;
  std::unique_ptr<export_command> _map_value_child;

  void apply_int_style(
      const std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>>
          &int_style
  ) {
    _int_style = int_style;

    if (_child && !_child->_int_style) _child->apply_int_style(int_style);
    if (_map_key_child && !_map_key_child->_int_style) _map_key_child->apply_int_style(int_style);
    if (_map_value_child && !_map_value_child->_int_style)
      _map_value_child->apply_int_style(int_style);
  }
};

const export_command export_command::default_command(_default_skip_size_func);

template <typename T>
struct value_with_command {
 public:
  const T &value;
  const export_command command;

  value_with_command(const T &v, export_command &&c) : value(v), command(std::move(c)) {}
  value_with_command() = delete;
};

template <typename>
inline constexpr bool _is_value_with_command = false;
template <typename T>
inline constexpr bool _is_value_with_command<value_with_command<T>> = true;

template <typename T>
inline constexpr bool is_value_with_command = _is_value_with_command<_remove_cref<T>>;

}  // namespace _detail

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto int_style(
    unsigned int base = 16,
    unsigned int digits = 8,
    unsigned int chunk = 2,
    bool space_fill = false,
    bool support_negative = false
) {
  return _detail::export_command({base, digits, chunk, space_fill, support_negative});
}

/*
 * Manipulator for the display style of decimal integers.
 * See README for details.
 */
inline auto int_style10(unsigned int digits, bool support_negative = false) {
  return int_style(10, digits, 0, true, support_negative);
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto show_front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &) -> std::optional<std::size_t> {
        if (index >= iteration_count) return std::nullopt;
        return 0;
      }
  );
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto show_back(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index,
          const std::function<std::size_t()> &get_size) -> std::optional<std::size_t> {
        std::size_t size = get_size();
        std::size_t first = size >= iteration_count ? size - iteration_count : 0;

        if (index < first) return first - index;
        return 0;
      }
  );
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto show_both_ends(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index,
          const std::function<std::size_t()> &get_size) -> std::optional<std::size_t> {
        std::size_t size = get_size();
        std::size_t first_half_last = (iteration_count + 1) / 2;
        std::size_t rest_count = iteration_count - first_half_last;
        std::size_t latter_half_first = size >= rest_count ? size - rest_count : 0;

        if (index >= first_half_last && index < latter_half_first) return latter_half_first - index;
        return 0;
      }
  );
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto show_middle(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index,
          const std::function<std::size_t()> &get_size) -> std::optional<std::size_t> {
        std::size_t size = get_size();
        std::size_t first = size >= iteration_count ? (size - iteration_count) / 2 : 0;
        std::size_t last = first + iteration_count;

        if (index < first) return first - index;
        if (index >= last) return std::nullopt;
        return 0;
      }
  );
}

/*
 * Manipulator for applying manipulators to map keys.
 * See README for details.
 */
inline auto map_k(_detail::export_command &&c) { return _map_key(std::move(c)); }

/*
 * Manipulator for applying manipulators to map values.
 * See README for details.
 */
inline auto map_v(_detail::export_command &&c) { return _map_value(std::move(c)); }

/*
 * Manipulator for applying manipulators to map keys and values.
 * See README for details.
 */
inline auto map_kv(_detail::export_command &&k, _detail::export_command &&v) {
  return _map_key_and_value(std::move(k), std::move(v));
}

}  // namespace cpp_dump
