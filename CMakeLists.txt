cmake_minimum_required(VERSION 3.5)
project(cpp-dump)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(-W4 /Zc:__cplusplus)
    set(rtti_flag "")
else()
    add_compile_options(-Wall -Wextra -Wfloat-equal -Wconversion -Wno-sign-conversion -Werror -Wshadow -fno-rtti)
    set(rtti_flag -frtti)
endif()

include(CTest)
enable_testing()

add_executable(static_test test/static_test.cpp test/odr_test.cpp)

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

add_executable(color_test test/color_test.cpp)
set(args_array "wide\;4000\;4" "narrow\;20\;4" "shallow\;4000\;0")
foreach(args ${args_array})
    list(GET args 0 suffix)

    add_test(
        NAME "color-${suffix}"
        COMMAND "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=$<TARGET_FILE:color_test>"
        -D "cmd_args=${args}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/color_test.cmake"
    )
endforeach()

if(CMAKE_CXX_STANDARD GREATER 17) 
    add_executable(color_std_version_test test/color_std_version_test.cpp)
    add_test(
        NAME "color-std-version"
        COMMAND "${CMAKE_COMMAND}"
        -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
        -D "cmd_path=$<TARGET_FILE:color_std_version_test>"
        -D "std_version=${CMAKE_CXX_STANDARD}"
        -D "compiler_id=${CMAKE_CXX_COMPILER_ID}"
        -P "${CMAKE_CURRENT_LIST_DIR}/test/color_std_version_test.cmake"
    )
endif()

add_executable(color_rtti_test test/color_rtti_test.cpp)
target_compile_options(color_rtti_test PRIVATE "${rtti_flag}")
add_test(
    NAME "color-rtti"
    COMMAND "${CMAKE_COMMAND}"
    -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
    -D "cmd_path=$<TARGET_FILE:color_rtti_test>"
    -P "${CMAKE_CURRENT_LIST_DIR}/test/color_rtti_test.cmake"
)

add_executable(log_label_test test/log_label_test.cpp)
add_test(
    NAME "log-label-test"
    COMMAND "${CMAKE_COMMAND}"
    -D "test_dir=${CMAKE_CURRENT_LIST_DIR}/test"
    -D "cmd_path=$<TARGET_FILE:log_label_test>"
    -P "${CMAKE_CURRENT_LIST_DIR}/test/log_label_test.cmake"
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
