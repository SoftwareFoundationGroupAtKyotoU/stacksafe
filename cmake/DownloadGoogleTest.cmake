set(DownloadGoogleTestConfigureFile "${CMAKE_CURRENT_LIST_DIR}/DownloadGoogleTest/CMakeLists.txt.in")

function(download_googletest)
  set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest")
  set(SOURCE_DIR "${TARGET_DIR}/src")
  set(BINARY_DIR "${TARGET_DIR}/build")

  configure_file("${DownloadGoogleTestConfigureFile}" "${TARGET_DIR}/CMakeLists.txt" @ONLY)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${TARGET_DIR}")
  if(result)
    message(FATAL_ERROR "Configure step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${TARGET_DIR}")
  if(result)
    message(FATAL_ERROR "Download step for googletest failed: ${result}")
  endif()
  add_subdirectory("${SOURCE_DIR}" "${BINARY_DIR}" EXCLUDE_FROM_ALL)
  enable_testing()
endfunction()
