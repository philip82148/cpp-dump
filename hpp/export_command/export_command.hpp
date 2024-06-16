/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>

#include "../options.hpp"
#include "../type_check.hpp"
#include "./skip_container.hpp"

namespace cpp_dump {

namespace _detail {

const std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)>
    _default_skip_size_func(
        [](std::size_t index, const std::function<std::size_t()> &) -> std::size_t {
          if (index >= max_iteration_count) return static_cast<std::size_t>(-1);
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

  explicit export_command(bool show_index) : _show_index(show_index) {}

  explicit export_command(int base, int digits, int chunk, bool space_fill, bool support_negative) {
    switch (base) {
      case 2:
      case 8:
      case 10:
      case 16:
        break;
      default:
        return;
    }
    if (digits < 0) digits = std::numeric_limits<int>::max();
    if (chunk < 0) chunk = 0;

    _int_style = {base, digits, chunk, space_fill, support_negative};
  }

  // This is for make_unique<export_command>().
  // Don't call this outside since this doesn't check if int_style is safe.
  explicit export_command(
      const std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>>
          &int_style,
      bool show_index
  )
      : _int_style(int_style), _show_index(show_index) {}

  explicit export_command(
      std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)> &&skip_size_func
  )
      : _skip_size_func(std::move(skip_size_func)) {}

  explicit export_command(
      const std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)>
          &skip_size_func
  )
      : _skip_size_func(skip_size_func) {}

  export_command(export_command &&) = default;
  export_command &operator=(export_command &&) = default;

  export_command(const export_command &c)
      : _int_style(c._int_style), _show_index(c._show_index), _skip_size_func(c._skip_size_func) {
    if (c._child) _child = std::make_unique<export_command>(*c._child);
    if (c._map_key_child) _map_key_child = std::make_unique<export_command>(*c._map_key_child);
    if (c._map_value_child)
      _map_value_child = std::make_unique<export_command>(*c._map_value_child);
  }

  export_command &operator=(const export_command &c) {
    _int_style = c._int_style;
    _show_index = c._show_index;
    _skip_size_func = c._skip_size_func;
    if (c._child) _child = std::make_unique<export_command>(*c._child);
    if (c._map_key_child) _child = std::make_unique<export_command>(*c._map_key_child);
    if (c._map_value_child) _child = std::make_unique<export_command>(*c._map_value_child);

    return *this;
  }

  export_command() = delete;

  export_command operator<<(export_command &&command) && {
    *this << std::move(command);
    return std::move(*this);
  }

  export_command &operator<<(export_command &&command) & {
    // export_command has int_style || show_index ||
    //     (either skip_size_func or {map_key_child || map_value_child}).
    // in the case of int_style
    if (command._int_style) _int_style = command._int_style;

    // in the case of show_index
    if (command._show_index) _show_index = command._show_index;

    // in the case of skip_size_func
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

    if (!command._map_key_child && !command._map_value_child) return *this;

    // in the case of {map_key_child || map_value_child}
    // jump to the node whose child has no skip_size_func.
    if (_child && _child->_skip_size_func) {
      *_child << std::move(command);
      return *this;
    }

    if (command._map_key_child) _map_key_child = std::move(command._map_key_child);
    if (command._map_value_child) _map_value_child = std::move(command._map_value_child);

    return *this;
  }

  export_command operator<<(const export_command &command) && {
    *this << export_command(command);
    return std::move(*this);
  }

  export_command operator<<(const export_command &command) const & {
    return export_command(*this) << export_command(command);
  }

  const export_command &next() const {
    if (_child) {
      if (_int_style && !_child->_int_style) _child->_int_style = _int_style;
      if (_show_index) _child->_show_index = _show_index;
    } else {
      if (!_int_style && !_show_index) return default_command;

      _child = std::make_unique<export_command>(_int_style, _show_index);
    }

    return *_child;
  }

  const export_command &next_for_map_key() const {
    if (_skip_size_func && _map_key_child) {
      if (_int_style && !_map_key_child->_int_style) _map_key_child->_int_style = _int_style;
      if (_show_index) _map_key_child->_show_index = _show_index;

      return *_map_key_child;
    }

    return next();
  }

  const export_command &next_for_map_value() const {
    if (_skip_size_func && _map_value_child) {
      if (_int_style && !_map_value_child->_int_style) _map_value_child->_int_style = _int_style;
      if (_show_index) _map_value_child->_show_index = _show_index;

      return *_map_value_child;
    }

    return next();
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) && {
    return value_with_command<T>(value, std::move(*this));
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) const & {
    return value_with_command<T>(value, *this);
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

  bool show_index() const { return _show_index; }

 private:
  std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>> _int_style;
  bool _show_index{false};
  std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)> _skip_size_func;
  mutable std::unique_ptr<export_command> _child;
  std::unique_ptr<export_command> _map_key_child;
  std::unique_ptr<export_command> _map_value_child;
};

inline const export_command export_command::default_command(_default_skip_size_func);

template <typename T>
struct value_with_command {
 public:
  const T &value;
  const export_command command;

  explicit value_with_command(const T &v, export_command &&c) : value(v), command(std::move(c)) {}
  explicit value_with_command(const T &v, const export_command &c) : value(v), command(c) {}
  value_with_command() = delete;
};

template <typename T>
value_with_command<T> operator|(const T &value, export_command &&command) {
  return value_with_command<T>(value, std::move(command));
}

template <typename T>
value_with_command<T> operator|(const T &value, const export_command &command) {
  return value_with_command<T>(value, command);
}

template <typename T>
value_with_command<T> operator|(value_with_command<T> &&vc, export_command &&command) {
  return value_with_command<T>(
      vc.value, const_cast<export_command &&>(vc.command) << std::move(command)
  );
}

template <typename T>
value_with_command<T> operator|(value_with_command<T> &&vc, const export_command &command) {
  return value_with_command<T>(vc.value, const_cast<export_command &&>(vc.command) << command);
}

template <typename T>
value_with_command<T> operator|(const value_with_command<T> &vc, export_command &&command) {
  return value_with_command<T>(vc.value, vc.command << std::move(command));
}

template <typename T>
value_with_command<T> operator|(const value_with_command<T> &vc, const export_command &command) {
  return value_with_command<T>(vc.value, vc.command << command);
}

inline export_command operator|(export_command &&lhs, export_command &&rhs) {
  return std::move(lhs) << std::move(rhs);
}

inline export_command operator|(const export_command &lhs, export_command &&rhs) {
  return lhs << std::move(rhs);
}

inline export_command operator|(export_command &&lhs, const export_command &rhs) {
  return std::move(lhs) << rhs;
}

inline export_command operator|(const export_command &lhs, const export_command &rhs) {
  return lhs << rhs;
}

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
    int base = 16,
    int digits = -1,
    int chunk = 2,
    bool space_fill = false,
    bool support_negative = false
) {
  return _detail::export_command(base, digits, chunk, space_fill, support_negative);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto dec(int digits = -1, int chunk = 0, bool support_negative = false) {
  return int_style(10, digits, chunk, true, support_negative);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto bin(int digits = -1, int chunk = 0, bool support_negative = false) {
  return int_style(2, digits, chunk, false, support_negative);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto oct(int digits = -1, int chunk = 0, bool support_negative = false) {
  return int_style(8, digits, chunk, false, support_negative);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto hex(int digits = -1, int chunk = 0, bool support_negative = false) {
  return int_style(16, digits, chunk, false, support_negative);
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &) -> std::size_t {
        if (index >= iteration_count) return static_cast<std::size_t>(-1);
        return 0;
      }
  );
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto back(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &cont_size) -> std::size_t {
        std::size_t size = cont_size();
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
inline auto both_ends(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &cont_size) -> std::size_t {
        std::size_t size = cont_size();
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
inline auto middle(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &cont_size) -> std::size_t {
        std::size_t size = cont_size();
        std::size_t first = size >= iteration_count ? (size - iteration_count) / 2 : 0;
        std::size_t last = first + iteration_count;

        if (index < first) return first - index;
        if (index >= last) return static_cast<std::size_t>(-1);
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

/*
 * Manipulator for the display style of containers.
 * See README for details.
 */
inline auto index() { return _detail::export_command(true); }

}  // namespace cpp_dump
