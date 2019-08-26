function(export_compile_commands)
  set(json_file "compile_commands.json")
  set(source_file "${CMAKE_BINARY_DIR}/${json_file}")
  set(target_file "${CMAKE_SOURCE_DIR}/${json_file}")

  if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    get_property(docstring CACHE CMAKE_EXPORT_COMPILE_COMMANDS PROPERTY HELPSTRING)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON
      CACHE BOOL "${docstring}" FORCE)
  endif()
  if(EXISTS "${source_file}")
    configure_file("${source_file}" "${target_file}" COPYONLY)
  endif()
endfunction()
