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
  list(JOIN CMAKE_CONFIGURATION_TYPES " " build_type_options)
  get_property(docstring CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING)
  string(REGEX REPLACE ":.*$" ": ${CMAKE_CONFIGURATION_TYPES}"
    build_type_doc "${docstring}")
  set(debug_flag_file "${CMAKE_CURRENT_LIST_DIR}/.debug")

  # determine default build type
  if ("${CMAKE_BUILD_TYPE}" IN_LIST CMAKE_CONFIGURATION_TYPES)
    set(build_type_default "${CMAKE_BUILD_TYPE}")
  else()
    if(EXISTS "${debug_flag_file}")
      set(build_type_default Debug)
    else()
      set(build_type_default Release)
    endif()
    if(CMAKE_BUILD_TYPE)
      message(WARNING
        "  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n"
        "  expected options: ${build_type_options}\n"
        "  fallback value  : ${build_type_default}\n")
    endif()
  endif()

  # set CMAKE_BUILD_TYPE
  set(CMAKE_BUILD_TYPE "${build_type_default}"
    CACHE STRING "${build_type_doc}" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS "${CMAKE_CONFIGURATION_TYPES}")
endfunction()
