function(update_submodule)
  set(option_list submodule update --init --recursive)
  string(REPLACE ";" " "
    options "${option_list}")

  find_package(Git REQUIRED)
  execute_process(COMMAND "${GIT_EXECUTABLE}" ${option_list}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE result)
  if(result)
    message(FATAL_ERROR "'git ${options}' failed with error code: ${result}")
  endif()
endfunction()
