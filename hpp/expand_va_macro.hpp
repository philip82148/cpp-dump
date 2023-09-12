/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#define CPP_DUMP_EXPAND_VA1_(func, arg1)       func(arg1)
#define CPP_DUMP_EXPAND_VA2_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA1_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA3_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA2_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA4_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA3_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA5_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA4_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA6_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA5_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA7_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA6_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA8_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA7_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA9_(func, arg1, ...)  func(arg1), CPP_DUMP_EXPAND_VA8_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA10_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA9_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA11_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA10_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA12_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA11_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA13_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA12_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA14_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA13_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA15_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA14_(func, __VA_ARGS__)
#define CPP_DUMP_EXPAND_VA16_(func, arg1, ...) func(arg1), CPP_DUMP_EXPAND_VA15_(func, __VA_ARGS__)

#define CPP_DUMP_VA_SIZE_AUX_(                                                                     \
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
#define CPP_DUMP_VA_SIZE_(...)                                                                     \
  CPP_DUMP_VA_SIZE_AUX_(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define CPP_DUMP_GET_EXPAND_VA_MACRO_(size) CPP_DUMP_EXPAND_VA##size##_
#define CPP_DUMP_EXPAND_VA_AUX_(func, size, ...)                                                   \
  CPP_DUMP_GET_EXPAND_VA_MACRO_(size)(func, __VA_ARGS__)

#define CPP_DUMP_EXPAND_VA_(func, ...)                                                             \
  CPP_DUMP_EXPAND_VA_AUX_(func, CPP_DUMP_VA_SIZE_(__VA_ARGS__), __VA_ARGS__)
