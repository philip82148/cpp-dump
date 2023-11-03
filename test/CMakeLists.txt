cmake_minimum_required(VERSION 3.0.0)
project(cpp-dump)

include(CTest)
enable_testing()

add_compile_options(-Wall -Wextra -Wfloat-equal -Wconversion -Wno-sign-conversion -Werror)

add_executable(dump_test ${CMAKE_SOURCE_DIR}/dump_test.cpp)

set(dump_test_path ${CMAKE_BINARY_DIR}/dump_test)
add_test(
    DumpTest ${CMAKE_COMMAND}
    -D dump_test_path=${dump_test_path}
    -D source_dir=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/dump_test.cmake
)
