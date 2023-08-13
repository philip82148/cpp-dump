#pragma once

#include "export_var.hpp"

#define __dump1(arg1) \
#arg1 " => " << [&]() -> std::string { try { return cpp_dump::export_var(arg1); } catch (std::exception &e) { return e.what(); } }()
#define __dump2(arg1, ...) __dump1(arg1) << ", " << __dump1(__VA_ARGS__)
#define __dump3(arg1, ...) __dump1(arg1) << ", " << __dump2(__VA_ARGS__)
#define __dump4(arg1, ...) __dump1(arg1) << ", " << __dump3(__VA_ARGS__)
#define __dump5(arg1, ...) __dump1(arg1) << ", " << __dump4(__VA_ARGS__)
#define __dump6(arg1, ...) __dump1(arg1) << ", " << __dump5(__VA_ARGS__)
#define __dump7(arg1, ...) __dump1(arg1) << ", " << __dump6(__VA_ARGS__)
#define __dump8(arg1, ...) __dump1(arg1) << ", " << __dump7(__VA_ARGS__)
#define __dump9(arg1, ...) __dump1(arg1) << ", " << __dump8(__VA_ARGS__)
#define __dump10(arg1, ...) __dump1(arg1) << ", " << __dump9(__VA_ARGS__)
#define __dump11(arg1, ...) __dump1(arg1) << ", " << __dump10(__VA_ARGS__)
#define __dump12(arg1, ...) __dump1(arg1) << ", " << __dump11(__VA_ARGS__)

#define __va_args_size_aux(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, \
                           arg9, arg10, arg11, arg12, size, ...)           \
  size
#define __va_args_size(...) \
  __va_args_size_aux(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define __get_dump_func(size) __dump##size
#define __dump(size, ...) \
  (clog << "[dump()] " << __get_dump_func(size)(__VA_ARGS__) << endl)

#define dump(...) __dump(__va_args_size(__VA_ARGS__), __VA_ARGS__)
