function(set_config_type)
  set(config_doc "possible options for CMAKE_BUILD_TYPE")
  set(CMAKE_CONFIGURATION_TYPES "${ARGV}"
    CACHE STRING "${config_doc}")
  mark_as_advanced(FORCE CMAKE_CONFIGURATION_TYPES)
endfunction()

function(set_build_type)
  # set CMAKE_CONFIGURATION_TYPES
  set_config_type(Release Debug)

  # setup constants
  set(build_type_default Release)
  set(build_type_list Release Debug)
  list(JOIN build_type_list " " build_type_options)
  get_property(docstring CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING)
  string(REGEX REPLACE ":.*$" ": ${build_type_options}"
    build_type_doc "${docstring}")
  set(config_doc "possible options for CMAKE_BUILD_TYPE")
  set(debug_flag_file "${CMAKE_SOURCE_DIR}/.debug")

  # determine default option
  if(EXISTS "${debug_flag_file}")
    set(build_type_default Debug)
  endif()
  if ("${CMAKE_BUILD_TYPE}" IN_LIST build_type_list)
    set(build_type_default "${CMAKE_BUILD_TYPE}")
  elseif(CMAKE_BUILD_TYPE)
    message(WARNING
      "  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n"
      "  expected options: ${build_type_options}\n"
      "  fallback value  : ${build_type_default}\n"
      )
  endif()

  # set global options
  set(CMAKE_BUILD_TYPE "${build_type_default}"
    CACHE STRING "${build_type_doc}" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endfunction()
