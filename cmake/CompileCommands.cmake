function(export_compile_commands)
  set(json_file "compile_commands.json")
  set(source_file "${CMAKE_BINARY_DIR}/${json_file}")
  set(target_file "${CMAKE_SOURCE_DIR}/${json_file}")

  include(OverrideCache)
  if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    override_cache(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  endif()
  if(EXISTS "${source_file}")
    configure_file("${source_file}" "${target_file}" COPYONLY)
  endif()
endfunction()
