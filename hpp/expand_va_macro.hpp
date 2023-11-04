/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

// This is for MSVC.
// See https://stackoverflow.com/questions/35210637/macro-expansion-argument-with-commas
#define _p_CPP_DUMP_CALL_VA_FUNC_AUX(x) x
#define _p_CPP_DUMP_CALL_VA_FUNC(func, args_with_paren)                                            \
  _p_CPP_DUMP_CALL_VA_FUNC_AUX(func args_with_paren)

#define _p_CPP_DUMP_VA_HEAD_AUX(arg1, ...) arg1
#define _p_CPP_DUMP_VA_HEAD(args_width_paren)                                                      \
  _p_CPP_DUMP_CALL_VA_FUNC(_p_CPP_DUMP_VA_HEAD_AUX, args_width_paren)
#define _p_CPP_DUMP_VA_TAIL_AUX(arg1, ...) (__VA_ARGS__)
#define _p_CPP_DUMP_VA_TAIL(args_width_paren)                                                      \
  _p_CPP_DUMP_CALL_VA_FUNC(_p_CPP_DUMP_VA_TAIL_AUX, args_width_paren)

#define _p_CPP_DUMP_BUFFERED_CALL(func, arg) func(arg)

#define _p_CPP_DUMP_EXPAND_VA1(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA2(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA1(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA3(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA2(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA4(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA3(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA5(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA4(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA6(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA5(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA7(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA6(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA8(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA7(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA9(func, args_with_paren)                                              \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA8(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA10(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA9(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA11(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA10(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA12(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA11(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA13(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA12(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA14(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA13(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA15(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA14(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))
#define _p_CPP_DUMP_EXPAND_VA16(func, args_with_paren)                                             \
  _p_CPP_DUMP_BUFFERED_CALL(func, _p_CPP_DUMP_VA_HEAD(args_with_paren)),                           \
      _p_CPP_DUMP_EXPAND_VA15(func, _p_CPP_DUMP_VA_TAIL(args_with_paren))

#define _p_CPP_DUMP_VA_SIZE_AUX(                                                                   \
    arg1,                                                                                          \
    arg2,                                                                                          \
    arg3,                                                                                          \
    arg4,                                                                                          \
    arg5,                                                                                          \
    arg6,                                                                                          \
    arg7,                                                                                          \
    arg8,                                                                                          \
    arg9,                                                                                          \
    arg10,                                                                                         \
    arg11,                                                                                         \
    arg12,                                                                                         \
    arg13,                                                                                         \
    arg14,                                                                                         \
    arg15,                                                                                         \
    arg16,                                                                                         \
    size,                                                                                          \
    ...                                                                                            \
)                                                                                                  \
  size
#define _p_CPP_DUMP_VA_SIZE(...)                                                                   \
  _p_CPP_DUMP_CALL_VA_FUNC(                                                                        \
      _p_CPP_DUMP_VA_SIZE_AUX,                                                                     \
      (__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)                         \
  )

#define _p_CPP_DUMP_GET_EXPAND_VA_MACRO(size) _p_CPP_DUMP_EXPAND_VA##size
#define _p_CPP_DUMP_EXPAND_VA_AUX(func, size, ...)                                                 \
  _p_CPP_DUMP_GET_EXPAND_VA_MACRO(size)(func, (__VA_ARGS__))

#define _p_CPP_DUMP_EXPAND_VA(func, ...)                                                           \
  _p_CPP_DUMP_EXPAND_VA_AUX(func, _p_CPP_DUMP_VA_SIZE(__VA_ARGS__), __VA_ARGS__)
