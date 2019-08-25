function(set_build_type)
  # setup constants
  set(build_type_default Release)
  set(build_type_list Release Debug)
  string(REPLACE ";" " " build_type_options "${build_type_list}")
  set(build_type_doc "Choose the type of build, options are: ${build_type_options}")
  set(debug_flag_file "${CMAKE_SOURCE_DIR}/.debug")

  # determine default option
  if(EXISTS "${debug_flag_file}")
    set(build_type_default Debug)
  endif()
  if ("${CMAKE_BUILD_TYPE}" IN_LIST build_type_list)
    set(build_type_default ${CMAKE_BUILD_TYPE})
  elseif(CMAKE_BUILD_TYPE)
    message(WARNING
      "  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n"
      "  expected options: ${build_type_options}\n"
      "  fallback value  : ${build_type_default}\n"
      )
  endif()

  # set global options
  set(CMAKE_CONFIGURATION_TYPES ${build_type_list})
  set(CMAKE_BUILD_TYPE ${build_type_default}
    CACHE STRING "${build_type_doc}" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS ${build_type_list})
endfunction()
