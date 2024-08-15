/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <cstdio>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

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
 public:
  struct int_style_t {
    unsigned int base;
    unsigned int digits;
    unsigned int chunk;
    bool space_fill;
    bool make_unsigned_or_no_space_for_minus;
  };
  enum class bool_style_t { normal, true_left, true_right, number };
  struct stresc {};
  struct charhex {};
  struct index {};

  struct global_props_t {
    std::optional<int_style_t> int_style;
    bool_style_t bool_style{bool_style_t::normal};
    const char *format{nullptr};
    std::size_t addr_depth{static_cast<std::size_t>(-1)};
    bool escape_str{false};
    bool char_as_hex{false};
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

      int_style = {
          static_cast<unsigned int>(base),
          static_cast<unsigned int>(digits),
          static_cast<unsigned int>(chunk),
          space_fill,
          make_unsigned_or_no_space_for_minus,
      };
    }
    explicit global_props_t(bool_style_t bool_style_) : bool_style(bool_style_) {}
    explicit global_props_t(const char *format_) : format(format_) {}
    explicit global_props_t(std::size_t addr_depth_) : addr_depth(addr_depth_) {}
    explicit global_props_t(stresc) : escape_str(true) {}
    explicit global_props_t(charhex) : char_as_hex(true) {}
    explicit global_props_t(index) : show_index(true) {}

    global_props_t(global_props_t &&) = default;
    global_props_t &operator=(global_props_t &&) = default;

    void update(const global_props_t &g) {
      if (g.int_style) int_style = g.int_style;
      if (g.bool_style != bool_style_t::normal) bool_style = g.bool_style;
      if (g.format != nullptr) format = g.format;
      if (g.escape_str) escape_str = g.escape_str;
      if (g.char_as_hex) char_as_hex = g.char_as_hex;
      if (g.show_index) show_index = g.show_index;
    }

    void merge(const global_props_t &g) {
      if (!int_style) int_style = g.int_style;
      if (bool_style == bool_style_t::normal) bool_style = g.bool_style;
      if (format == nullptr) format = g.format;
      if (!escape_str) escape_str = g.escape_str;
      if (!char_as_hex) char_as_hex = g.char_as_hex;
      if (!show_index) show_index = g.show_index;
    }
  };

  static const export_command default_command;

  export_command() = default;

  explicit export_command(
      int base, int digits, int chunk, bool space_fill, bool make_unsigned_or_no_space_for_minus
  )
      : _global_props(std::make_shared<global_props_t>(
          base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus
      )) {}

  explicit export_command(const char *format)
      : _global_props(std::make_shared<global_props_t>(format)) {}

  explicit export_command(bool_style_t bool_style)
      : _global_props(std::make_shared<global_props_t>(bool_style)) {}

  explicit export_command(std::size_t addr_depth)
      : _global_props(std::make_shared<global_props_t>(addr_depth)) {}

  explicit export_command(stresc) : _global_props(std::make_shared<global_props_t>(stresc{})) {}

  explicit export_command(charhex) : _global_props(std::make_shared<global_props_t>(charhex{})) {}

  explicit export_command(index) : _global_props(std::make_shared<global_props_t>(index{})) {}

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

  std::optional<int_style_t> int_style() const {
    return _global_props ? _global_props->int_style : std::nullopt;
  }

  bool_style_t bool_style() const {
    return _global_props ? _global_props->bool_style : bool_style_t::normal;
  }

  template <typename T>
  auto format(T value) const -> std::enable_if_t<is_arithmetic<T>, std::string> {
    if (!_global_props || _global_props->format == nullptr) return "";

    int sz = std::snprintf(nullptr, 0, _global_props->format, value);
    if (sz < 0) return "";

    std::vector<char> buffer(sz + 1);
    std::snprintf(buffer.data(), buffer.size(), _global_props->format, value);
    return {buffer.data(), static_cast<std::size_t>(sz)};
  }

  std::size_t addr_depth() const {
    return _global_props ? _global_props->addr_depth : static_cast<std::size_t>(-1);
  }

  bool escape_str() const { return _global_props && _global_props->escape_str; }

  bool char_as_hex() const { return _global_props && _global_props->char_as_hex; }

  bool show_index() const { return _global_props && _global_props->show_index; }

  // The below is for supporting lvalue of export_command -----------------------------------------

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

  // The above is for supporting lvalue of export_command -----------------------------------------

  friend export_command operator<<(export_command &&, export_command &&);
  friend export_command operator<<(export_command &&, const export_command &);
  friend export_command operator<<(const export_command &, export_command &&);
  friend export_command operator<<(const export_command &, const export_command &);
  friend export_command _map_k(export_command &&command);
  friend export_command _map_v(export_command &&command);
  friend export_command _map_kv(export_command &&key, export_command &&value);

 private:
  std::shared_ptr<global_props_t> _global_props;
  std::function<std::size_t(std::size_t, const std::function<std::size_t()> &)> _skip_size_func;

  // if export_command is used as rvalue, unique_ptr is enough.
  // Also, using shared_ptr might cause circular reference.
  mutable std::unique_ptr<export_command> _child;
  std::unique_ptr<export_command> _map_key_child;
  std::unique_ptr<export_command> _map_value_child;

  void update_and_append(export_command &&command) {
    update_global_props(std::move(command._global_props));
    append_child(std::move(command));
  }

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

inline const export_command export_command::default_command(_default_skip_size_func);

template <typename T>
struct value_with_command {
 public:
  const T &value;
  const export_command command;

  explicit value_with_command(const T &v, export_command &&c) : value(v), command(std::move(c)) {}
  value_with_command() = delete;
};

inline export_command operator<<(export_command &&lhs, export_command &&rhs) {
  lhs.update_and_append(std::move(rhs));
  return std::move(lhs);
}

template <typename T>
inline value_with_command<T> operator<<(export_command &&command, const T &value) {
  return value_with_command<T>(value, std::move(command));
}

// The below is for supporting lvalue of export_command -------------------------------------------

inline export_command operator<<(export_command &&lhs, const export_command &rhs) {
  lhs.update_and_append(export_command(rhs));
  return std::move(lhs);
}

inline export_command operator<<(const export_command &lhs, export_command &&rhs) {
  export_command retval(lhs);
  retval.update_and_append(std::move(rhs));
  return retval;
}

inline export_command operator<<(const export_command &lhs, const export_command &rhs) {
  export_command retval(lhs);
  retval.update_and_append(export_command(rhs));
  return retval;
}

template <typename T>
inline value_with_command<T> operator<<(const export_command &command, const T &value) {
  return value_with_command<T>(value, export_command(command));
}

// The above is for supporting lvalue of export_command -------------------------------------------

inline export_command operator|(export_command &&lhs, export_command &&rhs) {
  return std::move(lhs) << std::move(rhs);
}

template <typename T>
inline value_with_command<T> operator|(const T &value, export_command &&command) {
  return value_with_command<T>(value, std::move(command));
}

template <typename T>
inline value_with_command<T> operator|(value_with_command<T> &&vc, export_command &&command) {
  return value_with_command<T>(
      vc.value, const_cast<export_command &&>(vc.command) << std::move(command)
  );
}

// The below is for supporting lvalue of export_command -------------------------------------------

inline export_command operator|(const export_command &lhs, export_command &&rhs) {
  return lhs << std::move(rhs);
}

inline export_command operator|(export_command &&lhs, const export_command &rhs) {
  return std::move(lhs) << rhs;
}

inline export_command operator|(const export_command &lhs, const export_command &rhs) {
  return lhs << rhs;
}

template <typename T>
inline value_with_command<T> operator|(const T &value, const export_command &command) {
  return value_with_command<T>(value, export_command(command));
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

// The above is for supporting lvalue of export_command -------------------------------------------

inline export_command _map_k(export_command &&c) {
  export_command new_command;
  new_command._map_key_child = std::make_unique<export_command>(std::move(c));
  return new_command;
}

inline export_command _map_v(export_command &&c) {
  export_command new_command;
  new_command._map_value_child = std::make_unique<export_command>(std::move(c));
  return new_command;
}

inline export_command _map_kv(export_command &&key, export_command &&value) {
  export_command new_command;
  new_command._map_key_child = std::make_unique<export_command>(std::move(key));
  new_command._map_value_child = std::make_unique<export_command>(std::move(value));
  return new_command;
}

template <typename>
inline constexpr bool _is_value_with_command = false;
template <typename T>
inline constexpr bool _is_value_with_command<value_with_command<T>> = true;

template <typename T>
inline constexpr bool is_value_with_command = _is_value_with_command<_remove_cvref<T>>;

}  // namespace _detail

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto int_style(
    int base,
    int digits = -1,
    int chunk = 0,
    bool space_fill = false,
    bool make_unsigned_or_no_space_for_minus = false
) {
  return _detail::export_command(
      base, digits, chunk, space_fill, make_unsigned_or_no_space_for_minus
  );
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto dec(int digits = -1, int chunk = 0, bool space_fill = true) {
  return int_style(10, digits, chunk, space_fill, false);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto bin(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(2, digits, chunk, space_fill, false);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto oct(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(8, digits, chunk, space_fill, false);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto hex(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(16, digits, chunk, space_fill, false);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto udec(int digits = -1, int chunk = 0, bool space_fill = true) {
  return int_style(10, digits, chunk, space_fill, true);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto ubin(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(2, digits, chunk, space_fill, true);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto uoct(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(8, digits, chunk, space_fill, true);
}

/**
 * Manipulator for the display style of integers.
 * See README for details.
 */
inline auto uhex(int digits = -1, int chunk = 0, bool space_fill = false) {
  return int_style(16, digits, chunk, space_fill, true);
}

/**
 * Manipulator for the display style of numbers.
 * See README for details.
 */
inline auto format(const char *f) { return _detail::export_command(f); }

/**
 * Manipulator for the display style of bool.
 * See README for details.
 */
inline auto bw(bool left = false) {
  using bool_style_t = _detail::export_command::bool_style_t;
  return _detail::export_command(left ? bool_style_t::true_left : bool_style_t::true_right);
}

/**
 * Manipulator for the display style of bool.
 * See README for details.
 */
inline auto boolnum() {
  using bool_style_t = _detail::export_command::bool_style_t;
  return _detail::export_command(bool_style_t::number);
}

/**
 * Manipulator for the display style of char.
 * See README for details.
 */
inline auto stresc() { return _detail::export_command(_detail::export_command::stresc{}); }

/**
 * Manipulator for the display style of char.
 * See README for details.
 */
inline auto charhex() { return _detail::export_command(_detail::export_command::charhex{}); }

/**
 * Manipulator for the display style of pointers.
 * See README for details.
 */
inline auto addr(std::size_t depth = 0) { return _detail::export_command(depth); }

/**
 * Manipulator for the display style of containers.
 * See README for details.
 */
inline auto index() { return _detail::export_command(_detail::export_command::index{}); }

/**
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

/**
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

/**
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

/**
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

/**
 * Manipulator for applying manipulators to map keys.
 * See README for details.
 */
inline auto map_k(_detail::export_command &&c) { return _detail::_map_k(std::move(c)); }

/**
 * Manipulator for applying manipulators to map values.
 * See README for details.
 */
inline auto map_v(_detail::export_command &&c) { return _detail::_map_v(std::move(c)); }

/**
 * Manipulator for applying manipulators to map keys and values.
 * See README for details.
 */
inline auto map_kv(_detail::export_command &&k, _detail::export_command &&v) {
  return _detail::_map_kv(std::move(k), std::move(v));
}

}  // namespace cpp_dump
