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
#include <string>
#include <string_view>
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
    export_command new_command;
    new_command._map_key_child = std::make_unique<export_command>(std::move(command));
    return new_command;
  }

  friend export_command _map_value(export_command &&command) {
    export_command new_command;
    new_command._map_value_child = std::make_unique<export_command>(std::move(command));
    return new_command;
  }

  friend export_command _map_key_and_value(export_command &&key, export_command &&value) {
    export_command new_command;
    new_command._map_key_child = std::make_unique<export_command>(std::move(key));
    new_command._map_value_child = std::make_unique<export_command>(std::move(value));
    return new_command;
  }

 public:
  struct global_props_t {
    std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>> int_style;
    std::string format;
    bool show_index{false};

    explicit global_props_t(
        int base, int digits, int chunk, bool space_fill, bool make_unsigned_or_no_space_for_minus
    ) {
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

      int_style = {base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus};
    }
    explicit global_props_t(std::string_view _format) : format(_format) {}
    explicit global_props_t(bool _show_index) : show_index(_show_index) {}

    global_props_t(global_props_t &&) = default;
    global_props_t &operator=(global_props_t &&) = default;

    void update(const global_props_t &g) {
      if (g.int_style) int_style = g.int_style;
      if (!g.format.empty()) format = g.format;
      if (g.show_index) show_index = g.show_index;
    }

    void merge(const global_props_t &g) {
      if (!int_style) int_style = g.int_style;
      if (format.empty()) format = g.format;
      if (!show_index) show_index = g.show_index;
    }
  };

  static const export_command default_command;

  export_command() = default;

  explicit export_command(bool show_index)
      : _global_props(std::make_shared<global_props_t>(show_index)) {}

  explicit export_command(
      int base, int digits, int chunk, bool space_fill, bool make_unsigned_or_no_space_for_minus
  )
      : _global_props(std::make_shared<global_props_t>(
          base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus
      )) {}

  explicit export_command(std::string_view format)
      : _global_props(std::make_shared<global_props_t>(format)) {}

  explicit export_command(const std::shared_ptr<global_props_t> &g) : _global_props(g) {}

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

  void update_and_append(export_command &&command) {
    update_global_props(std::move(command._global_props));
    append_child(std::move(command));
  }

  const export_command &next() const {
    if (_child) {
      if (_global_props && !_child->_global_props) _child->_global_props = _global_props;
    } else {
      if (!_global_props) return default_command;

      _child = std::make_unique<export_command>(_global_props);
    }

    return *_child;
  }

  const export_command &next_for_map_key() const {
    if (!_skip_size_func) return next();  // this is 0th dim.
    if (!_map_key_child) return next();

    if (_global_props) {
      if (_map_key_child->_global_props) {
        _map_key_child->_global_props->merge(*_global_props);
      } else {
        _map_key_child->_global_props = _global_props;
      }
    }

    return *_map_key_child;
  }

  const export_command &next_for_map_value() const {
    if (!_skip_size_func) return next();  // this is 0th dim.
    if (!_map_value_child) return next();

    if (_global_props) {
      if (_map_value_child->_global_props) {
        _map_value_child->_global_props->merge(*_global_props);
      } else {
        _map_value_child->_global_props = _global_props;
      }
    }

    return *_map_value_child;
  }

  template <typename T>
  skip_container<T> create_skip_container(const T &container) const {
    if (_skip_size_func) return skip_container<T>(container, _skip_size_func);
    return skip_container<T>(container, _default_skip_size_func);
  }

  std::optional<std::tuple<unsigned int, unsigned int, unsigned int, bool, bool>> int_style(
  ) const {
    return _global_props ? _global_props->int_style : std::nullopt;
  }

  bool show_index() const { return _global_props && _global_props->show_index; }

  // The below is for supporting lvalue of export_command.

  export_command(const export_command &c)
      : _global_props(c._global_props), _skip_size_func(c._skip_size_func) {
    if (c._child) _child = std::make_unique<export_command>(*c._child);
    if (c._map_key_child) _map_key_child = std::make_unique<export_command>(*c._map_key_child);
    if (c._map_value_child)
      _map_value_child = std::make_unique<export_command>(*c._map_value_child);
  }

  export_command &operator=(const export_command &c) {
    _global_props = c._global_props;
    _skip_size_func = c._skip_size_func;
    if (c._child) _child = std::make_unique<export_command>(*c._child);
    if (c._map_key_child) _child = std::make_unique<export_command>(*c._map_key_child);
    if (c._map_value_child) _child = std::make_unique<export_command>(*c._map_value_child);

    return *this;
  }

 private:
  std::shared_ptr<global_props_t> _global_props;
  std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)> _skip_size_func;

  // if export_command is used as rvalue, unique_ptr is enough.
  // Also, using shared_ptr might cause circular reference.
  mutable std::unique_ptr<export_command> _child;
  std::unique_ptr<export_command> _map_key_child;
  std::unique_ptr<export_command> _map_value_child;

  void update_global_props(std::shared_ptr<global_props_t> &&g) {
    if (g) {
      if (_global_props) {
        _global_props->update(*g);
        g.reset();
      } else {
        _global_props.swap(g);
      }
    }

    // Here, g == nullptr
  }

  void append_child(export_command &&command) {
    // export_command has (either skip_size_func or {map_key_child || map_value_child})( || g).

    // in the case of skip_size_func
    if (command._skip_size_func) {
      if (_skip_size_func) {
        // append
        if (_child) {
          _child->append_child(std::move(command));
        } else {
          _child = std::make_unique<export_command>(std::move(command));
        }

        return;
      }

      // update this node
      _skip_size_func = std::move(command._skip_size_func);
      if (command._map_key_child) _map_key_child = std::move(command._map_key_child);
      if (command._map_value_child) _map_value_child = std::move(command._map_value_child);
      return;
    }

    if (!(command._map_key_child || command._map_value_child)) return;

    // in the case of {map_key_child || map_value_child}
    // jump to the node whose child has no skip_size_func.
    if (_child && _child->_skip_size_func) {
      _child->append_child(std::move(command));
      return;
    }

    if (command._map_key_child) _map_key_child = std::move(command._map_key_child);
    if (command._map_value_child) _map_value_child = std::move(command._map_value_child);
  }
};

inline export_command operator<<(export_command &&lhs, export_command &&rhs) {
  lhs.update_and_append(std::move(rhs));
  return std::move(lhs);
}

template <typename T>
inline value_with_command<T> operator<<(export_command &&lhs, const T &rhs) {
  return value_with_command<T>(rhs, std::move(lhs));
}

inline export_command operator<<(export_command &&lhs, const export_command &rhs) {
  lhs.update_and_append(export_command(rhs));
  return std::move(lhs);
}

inline export_command operator<<(const export_command &lhs, const export_command &rhs) {
  export_command retval(lhs);
  retval.update_and_append(export_command(rhs));
  return retval;
}

template <typename T>
inline value_with_command<T> operator<<(const export_command &lhs, const T &value) {
  return value_with_command<T>(value, lhs);
}

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
inline value_with_command<T> operator|(const T &value, export_command &&command) {
  return value_with_command<T>(value, std::move(command));
}

template <typename T>
inline value_with_command<T> operator|(const T &value, const export_command &command) {
  return value_with_command<T>(value, command);
}

template <typename T>
inline value_with_command<T> operator|(value_with_command<T> &&vc, export_command &&command) {
  return value_with_command<T>(
      vc.value, const_cast<export_command &&>(vc.command) << std::move(command)
  );
}

template <typename T>
inline value_with_command<T> operator|(value_with_command<T> &&vc, const export_command &command) {
  return value_with_command<T>(vc.value, const_cast<export_command &&>(vc.command) << command);
}

template <typename T>
inline value_with_command<T> operator|(const value_with_command<T> &vc, export_command &&command) {
  return value_with_command<T>(vc.value, vc.command << std::move(command));
}

template <typename T>
inline value_with_command<T> operator|(
    const value_with_command<T> &vc, const export_command &command
) {
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
inline constexpr bool is_value_with_command = _is_value_with_command<_remove_cvref<T>>;

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
    bool make_unsigned_or_no_space_for_minus = false
) {
  return _detail::export_command(
      base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus
  );
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto dec(int digits = -1, int chunk = 0) {
  return int_style(10, digits, chunk, true, false);
}

/*
 * Manipulator for the display style of integers.
 * This is an experimental feature.
 */
inline auto udec(int digits = -1, int chunk = 0) {
  return int_style(10, digits, chunk, true, true);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto bin(int digits = -1, int chunk = 0) {
  return int_style(2, digits, chunk, false, false);
}

/*
 * Manipulator for the display style of integers.
 * This is an experimental feature.
 */
inline auto ubin(int digits = -1, int chunk = 0) {
  return int_style(2, digits, chunk, false, true);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto oct(int digits = -1, int chunk = 0) {
  return int_style(8, digits, chunk, false, false);
}

/*
 * Manipulator for the display style of integers.
 * This is an experimental feature.
 */
inline auto uoct(int digits = -1, int chunk = 0) {
  return int_style(8, digits, chunk, false, true);
}

/*
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto hex(int digits = -1, int chunk = 0) {
  return int_style(16, digits, chunk, false, false);
}

/*
 * Manipulator for the display style of integers.
 * This is an experimental feature.
 */
inline auto uhex(int digits = -1, int chunk = 0) {
  return int_style(16, digits, chunk, false, true);
}

/*
 * Manipulator for the display style of integers.
 * This is an experimental feature.
 */
inline auto format(std::string_view f) { return _detail::export_command(f); }

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &) -> std::size_t {
        if (index >= iteration_count) return static_cast<std::size_t>(-1);  // skip to the end

        return 0;  // increment normally
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

        if (index < first) return first - index;  // skip to the first

        return 0;  // increment normally
      }
  );
}

/*
 * Manipulator for the display style of iterables.
 * See README for details.
 */
inline auto both_ends(std::size_t half_iteration_count = max_iteration_count / 2) {
  return _detail::export_command(
      [=](std::size_t index, const std::function<std::size_t()> &cont_size) -> std::size_t {
        std::size_t size = cont_size();
        std::size_t latter_half_first =
            size >= half_iteration_count ? size - half_iteration_count : 0;

        if (index >= half_iteration_count && index < latter_half_first)
          return latter_half_first - index;  // skip to the latter_half_first

        return 0;  // increment normally
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

        if (index < first) return first - index;                 // skip to the first
        if (index >= last) return static_cast<std::size_t>(-1);  // skip to the end

        return 0;  // increment normally
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
