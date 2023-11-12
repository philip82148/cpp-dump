cmake_minimum_required(VERSION 3.5)
project(cpp-dump)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(-W4 /Zc:__cplusplus)
else()
    add_compile_options(-Wall -Wextra -Wfloat-equal -Wconversion -Wno-sign-conversion -Werror -Wshadow)
endif()

include(CTest)
enable_testing()

add_executable(dump_test test/dump_test.cpp)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_definitions(dump_test PRIVATE USE_BITS_STDC)
endif()
set(args_array "normal\;160\;4" "narrow\;20\;4" "wide\;4000\;4" "shallow\;4000\;0")
foreach(args ${args_array})
    list(GET args 0 suffix)

    add_test(
        NAME "dump-${suffix}"
        COMMAND "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=$<TARGET_FILE:dump_test>"
        -D "cmd_args=${args}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/dump_test.cmake"
    )
endforeach()

add_executable(dump_variable_test test/dump_variable_test.cpp)
add_test(
    NAME "dump-variable"
    COMMAND "${CMAKE_COMMAND}"
    -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
    -D "cmd_path=$<TARGET_FILE:dump_variable_test>"
    -D "compiler_id=${CMAKE_CXX_COMPILER_ID}"
    -P "${CMAKE_CURRENT_LIST_DIR}/test/dump_variable_test.cmake"
)

file(GLOB files readme/*.cpp)
foreach(file ${files})
    get_filename_component(basename "${file}" NAME_WLE)

    add_executable("readme_${basename}" "${file}")
    add_test(
        NAME "readme-${basename}"
        COMMAND "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=$<TARGET_FILE:readme_${basename}>"
        -D "compiler_id=${CMAKE_CXX_COMPILER_ID}"
        -D "basename=${basename}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/readme_test.cmake"
    )
endforeach()
