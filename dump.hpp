#pragma once

#include "export_var.hpp"

#define _dump1(arg1) \
#arg1 " => " << [&]() -> string { try { return cpp_dump::export_var(arg1); } catch (exception &e) { return e.what(); } }()
#define _dump2(arg1, ...) _dump1(arg1) << ", " << _dump1(__VA_ARGS__)
#define _dump3(arg1, ...) _dump1(arg1) << ", " << _dump2(__VA_ARGS__)
#define _dump4(arg1, ...) _dump1(arg1) << ", " << _dump3(__VA_ARGS__)
#define _dump5(arg1, ...) _dump1(arg1) << ", " << _dump4(__VA_ARGS__)
#define _dump6(arg1, ...) _dump1(arg1) << ", " << _dump5(__VA_ARGS__)
#define _dump7(arg1, ...) _dump1(arg1) << ", " << _dump6(__VA_ARGS__)
#define _dump8(arg1, ...) _dump1(arg1) << ", " << _dump7(__VA_ARGS__)
#define _dump9(arg1, ...) _dump1(arg1) << ", " << _dump8(__VA_ARGS__)
#define _dump10(arg1, ...) _dump1(arg1) << ", " << _dump9(__VA_ARGS__)
#define _dump11(arg1, ...) _dump1(arg1) << ", " << _dump10(__VA_ARGS__)
#define _dump12(arg1, ...) _dump1(arg1) << ", " << _dump11(__VA_ARGS__)

#define _va_args_size_aux(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, \
                          arg9, arg10, arg11, arg12, size, ...)           \
  size
#define _va_args_size(...) \
  _va_args_size_aux(__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define _get_dump_func(size) _dump##size
#define _dump(size, ...) \
  clog << "[dump()] " << _get_dump_func(size)(__VA_ARGS__) << endl

#define dump(...) _dump(_va_args_size(__VA_ARGS__), __VA_ARGS__)
