/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../options.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"
#include "./export_var_fwd.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
struct _set_dummy_wrapper {
 public:
  explicit _set_dummy_wrapper(const T &set) : _set(set) {}

  auto begin() const noexcept { return _set.begin(); }
  auto end() const noexcept { return _set.end(); }
  auto size() const noexcept { return _set.size(); }

 private:
  const T &_set;
};

template <typename T>
struct _multiset_wrapper {
 public:
  explicit _multiset_wrapper(const T &set) : _set(set) {}

  auto begin() const noexcept { return multiset_wrapper_iterator(_set, _set.begin()); }
  auto end() const noexcept { return multiset_wrapper_iterator(_set, _set.end()); }

 private:
  struct multiset_wrapper_iterator {
   public:
    using It = typename T::const_iterator;
    multiset_wrapper_iterator(const T &set, It it) : _set(set), _it(it) {}

    const auto &operator*() const noexcept { return *_it; }
    bool operator!=(const multiset_wrapper_iterator &to) const noexcept { return _it != to._it; }
    multiset_wrapper_iterator &operator++() {
      _it = _set.equal_range(*_it).second;
      return *this;
    }

   private:
    const T &_set;
    It _it;
  };

  const T &_set;
};

template <typename T>
inline auto export_set(
    const T &set,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_set<T>, std::string> {
  if (set.empty()) return es::bracket("{ }", current_depth);

  if (current_depth >= options::max_depth)
    return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

  std::size_t next_depth = current_depth + 1;
  const auto &next_command = command.next();

  auto set_wrapper = ([&]() {
    if constexpr (is_multiset<T>) {
      return _multiset_wrapper(set);
    } else {
      // The wrapper is to avoid calling the copy constructor.
      return _set_dummy_wrapper(set);
    }
  })();
  auto skipped_set = command.create_skip_container(set_wrapper);

  bool shift_indent;
  if (options::cont_indent_style == types::cont_indent_style_t::always) {
    shift_indent = true;
  } else if (options::cont_indent_style == types::cont_indent_style_t::when_nested) {
    shift_indent = is_iterable_like<iterable_elem_type<T>>;
  } else if (options::cont_indent_style == types::cont_indent_style_t::when_non_tuples_nested) {
    shift_indent = is_iterable_like<iterable_elem_type<T>> && !is_tuple<iterable_elem_type<T>>;
  } else {
    shift_indent = false;
  }

  if (!shift_indent) {
    std::string output = es::bracket("{ ", current_depth);
    bool is_first_elem = true;

    for (const auto &[is_ellipsis, it, _index] : skipped_set) {
      [[maybe_unused]] const auto &_index_unused = _index;  // for g++-7 compiler support
      const auto &elem = *it;

      if (is_first_elem) {
        is_first_elem = false;
      } else {
        output += es::op(", ");
      }

      if (is_ellipsis) {
        output += es::op("...");

        if (last_line_length + get_length(output) + std::string_view(" }").size()
            > options::max_line_width) {
          shift_indent = true;
          break;
        }

        continue;
      }

      std::string elem_str = export_var(
          elem, indent, last_line_length + get_length(output), next_depth, true, next_command
      );

      // Treat the multiplicity as a member as export_map() does.
      if constexpr (is_multiset<T>)
        elem_str += es::member(" (" + std::to_string(set.count(elem)) + ")");

      if (has_newline(elem_str)) {
        shift_indent = true;
        break;
      }

      output += elem_str;
      if (last_line_length + get_length(output) + std::string_view(" }").size()
          > options::max_line_width) {
        shift_indent = true;
        break;
      }
    }

    if (!shift_indent) {
      output += es::bracket(" }", current_depth);

      return output;
    }
  }

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  std::string output = es::bracket("{", current_depth);
  bool is_first_elem = true;

  for (const auto &[is_ellipsis, it, _index] : skipped_set) {
    [[maybe_unused]] const auto &_index_unused = _index;  // for g++-7 compiler support
    const auto &elem = *it;

    if (is_first_elem) {
      is_first_elem = false;
    } else {
      output += es::op(",");
    }

    if (is_ellipsis) {
      output += "\n" + new_indent + es::op("...");
      continue;
    }

    output += "\n" + new_indent
              + export_var(elem, new_indent, new_indent.length(), next_depth, false, next_command);

    // Treat the multiplicity as a member as export_map() does.
    if constexpr (is_multiset<T>)
      output += es::member(" (" + std::to_string(set.count(elem)) + ")");
  }
  output += "\n" + indent + es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
