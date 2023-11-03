cmake_policy(SET CMP0057 NEW)

if(NOT source_dir)
   message(FATAL_ERROR "Variable source_dir not defined" )
endif()
if(NOT cmd_path)
   message(FATAL_ERROR "Variable dump_test_path not defined" )
endif()
if(NOT basename)
   message(FATAL_ERROR "Variable dump_test_path not defined" )
endif()

file(MAKE_DIRECTORY ${source_dir}/log)

set(log_file ${source_dir}/log/readme_${basename}.log)
set(txt_file ${source_dir}/txt/readme_${basename}.txt)

execute_process(
   COMMAND ${cmd_path} ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)

# Do not remove escape sequences but remove raw addresses.
set(raw_address_file supports-various-types;customizable-colors;no-es)
if (${basename} IN_LIST raw_address_file)
   string(REGEX REPLACE "0x[0-9a-f]*" "" error_contents ${error_contents})
endif()
file(WRITE ${log_file} ${error_contents})

execute_process(
   COMMAND ${CMAKE_COMMAND} -E compare_files ${log_file} ${txt_file} RESULT_VARIABLE not_successful COMMAND_ERROR_IS_FATAL ANY
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file}!" )
endif()
