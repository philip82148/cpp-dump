if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined" )
endif()
if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined" )
endif()
if(NOT std_version)
   message(FATAL_ERROR "Variable std_version not defined" )
endif()
if(NOT compiler_id)
   message(FATAL_ERROR "Variable compiler_id not defined" )
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

if(compiler_id MATCHES "GNU")
   set(log_file "${test_dir}/log/color_std_version_${std_version}_gnu.log")
   set(txt_file "${test_dir}/txt/color_std_version_${std_version}_gnu.txt")
elseif(compiler_id MATCHES "MSVC")
   set(log_file "${test_dir}/log/color_std_version_${std_version}_msvc.log")
   set(txt_file "${test_dir}/txt/color_std_version_${std_version}_msvc.txt")
else()
   set(log_file "${test_dir}/log/color_std_version_${std_version}_clang.log")
   set(txt_file "${test_dir}/txt/color_std_version_${std_version}_clang.txt")
endif()

execute_process(
   COMMAND "${cmd_path}" ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "\"[^}]*\\.cpp" "\"" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)
if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file} !" )
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
