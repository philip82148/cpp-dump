if(NOT dump_test_path)
   message(FATAL_ERROR "Variable dump_test_path not defined" )
endif()
if(NOT source_dir)
   message(FATAL_ERROR "Variable source_dir not defined" )
endif()

file(MAKE_DIRECTORY ${source_dir}/log)

set(args_array "dump\;160\;4" "dump_narrow\;20\;4" "dump_wide\;4000\;4" "dump_shallow\;4000\;0")
string(ASCII 27 esc)
foreach(args ${args_array})
   list(GET args 0 basename)
   list(GET args 1 width)
   list(GET args 2 depth)
   set(log_file ${source_dir}/log/${basename}.log)
   set(txt_file ${source_dir}/txt/${basename}.txt)

   # no color
   execute_process(
      COMMAND ${dump_test_path} ${width} ${depth} 0 ERROR_FILE ${log_file} COMMAND_ERROR_IS_FATAL ANY
   )
   execute_process(
      COMMAND ${CMAKE_COMMAND} -E compare_files ${log_file} ${txt_file} RESULT_VARIABLE not_successful COMMAND_ERROR_IS_FATAL ANY
   )
   if(not_successful)
      message(SEND_ERROR "${log_file} does not match ${txt_file}!" )
   endif()

   # with color
   execute_process(
      COMMAND ${dump_test_path} ${width} ${depth} 1 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
   )
   string(REGEX REPLACE "${esc}\\[[^m]*m" "" error_contents ${error_contents})
   file(WRITE ${log_file} ${error_contents})
   execute_process(
      COMMAND ${CMAKE_COMMAND} -E compare_files ${log_file} ${txt_file} RESULT_VARIABLE not_successful COMMAND_ERROR_IS_FATAL ANY
   )
   if(not_successful)
      message(SEND_ERROR "${log_file} with color does not match ${txt_file}!" )
   endif()
endforeach()
