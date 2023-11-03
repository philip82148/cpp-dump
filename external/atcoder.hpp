#include <atcoder/modint>
#include <string>

#include "../dump.hpp"

namespace cpp_dump {

namespace _detail {

template <int m>
inline constexpr bool _is_external<atcoder::static_modint<m>> = true;

template <int m>
std::string export_var(
    const atcoder::static_modint<m> &mint,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_var(mint.val(), indent, last_line_length, current_depth, fail_on_newline, command);
}

template <int m>
inline constexpr bool _is_external<atcoder::dynamic_modint<m>> = true;

template <int m>
std::string export_var(
    const atcoder::dynamic_modint<m> &mint,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) {
  return export_var(mint.val(), indent, last_line_length, current_depth, fail_on_newline, command);
}

}  // namespace _detail

}  // namespace cpp_dump
