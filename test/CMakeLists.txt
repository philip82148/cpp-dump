cmake_minimum_required(VERSION 3.3.0)
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

file(GLOB files ${CMAKE_SOURCE_DIR}/../readme/*.cpp)
foreach(file ${files})
    get_filename_component(basename ${file} NAME_WLE)
    add_executable(readme_${basename} ${file})
    set(cmd_path ${CMAKE_BINARY_DIR}/readme_${basename})
    add_test(
        ReadmeTest-${basename} ${CMAKE_COMMAND}
        -D source_dir=${CMAKE_SOURCE_DIR}
        -D cmd_path=${cmd_path}
        -D basename=${basename}
        -P ${CMAKE_SOURCE_DIR}/readme_test.cmake
    )
endforeach()
