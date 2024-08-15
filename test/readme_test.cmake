cmake_policy(SET CMP0057 NEW)

if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

if(NOT compiler_id)
   message(FATAL_ERROR "Variable compiler_id not defined")
endif()

if(NOT basename)
   message(FATAL_ERROR "Variable basename not defined")
endif()

include("${test_dir}/common.cmake")

file(MAKE_DIRECTORY "${test_dir}/log")

set(log_file "${test_dir}/log/readme_${basename}.log")
set(txt_file "${test_dir}/txt/readme_${basename}.txt")

execute_process(
   COMMAND "${cmd_path}" ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)

set(raw_address_file supports-various-types;customizable-colors;no-es;formatting-with-manipulators)

if("${basename}" IN_LIST raw_address_file)
   # Do not remove escape sequences but remove raw addresses.
   if("${basename}" STREQUAL no-es)
      string(REGEX REPLACE " (0x[0-9a-f]+|00[0-9A-F]+)," " 0x7fffffffffff," error_contents "${error_contents}")
   else()
      string(REGEX REPLACE "${esc0x1b}\\[32m(0x[0-9a-f]+|00[0-9A-F]+)${esc0x1b}\\[0m" "${esc0x1b}[32m0x7fffffffffff${esc0x1b}[0m" error_contents "${error_contents}")
   endif()
elseif("${basename}" STREQUAL "user-defined-class2")
   string(REGEX REPLACE "main.*::.*class_A" "class_A" error_contents "${error_contents}")
endif()

file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} does not match ${txt_file} !")
