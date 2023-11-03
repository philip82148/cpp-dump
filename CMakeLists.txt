cmake_minimum_required(VERSION 3.3.0)
project(cpp-dump)

add_compile_options(-Wall -Wextra -Wfloat-equal -Wconversion -Wno-sign-conversion -Werror)

include(CTest)
enable_testing()

add_executable(dump_test test/dump_test.cpp)
set(args_array "normal\;160\;4" "narrow\;20\;4" "wide\;4000\;4" "shallow\;4000\;0")
foreach(args ${args_array})
    list(GET args 0 suffix)

    add_test(
        "dump-${suffix}"
        "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=${CMAKE_CURRENT_BINARY_DIR}/dump_test"
        -D "cmd_args=${args}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/dump_test.cmake"
    )
endforeach()

file(GLOB files readme/*.cpp)
foreach(file ${files})
    get_filename_component(basename "${file}" NAME_WLE)

    add_executable("readme_${basename}" "${file}")
    add_test(
        "readme-${basename}"
        "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=${CMAKE_CURRENT_BINARY_DIR}/readme_${basename}"
        -D "basename=${basename}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/readme_test.cmake"
    )
endforeach()
