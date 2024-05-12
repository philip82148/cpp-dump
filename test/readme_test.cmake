cmake_policy(SET CMP0057 NEW)

if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined" )
endif()
if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined" )
endif()
if(NOT compiler_id)
   message(FATAL_ERROR "Variable compiler_id not defined" )
endif()
if(NOT basename)
   message(FATAL_ERROR "Variable basename not defined" )
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

set(log_file "${test_dir}/log/readme_${basename}.log")
set(txt_file "${test_dir}/txt/readme_${basename}.txt")

execute_process(
   COMMAND "${cmd_path}" ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)

set(raw_address_file supports-various-types;customizable-colors;no-es)
if ("${basename}" IN_LIST raw_address_file)
   # Do not remove escape sequences but remove raw addresses.
   if("${basename}" STREQUAL no-es)
      string(REGEX REPLACE " (0x[0-9a-f]+|00[0-9A-F]+)," " ," error_contents "${error_contents}")
   else()
      string(REGEX REPLACE "${esc}\\[32m(0x[0-9a-f]+|00[0-9A-F]+)${esc}\\[0m" "${esc}[32m${esc}[0m" error_contents "${error_contents}")
   endif()
elseif("${basename}" STREQUAL "user-defined-class2")
   if(compiler_id MATCHES "GNU")
      string(REGEX REPLACE "main\\(\\)::" "" error_contents "${error_contents}")
   elseif(compiler_id MATCHES "MSVC")
      string(REGEX REPLACE "main::" "" error_contents "${error_contents}")
   endif()
endif()
file(WRITE "${log_file}" "${error_contents}")

execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !" )
endif()
