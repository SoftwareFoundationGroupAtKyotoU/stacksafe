project(pass CXX)

set(target "${CMAKE_PROJECT_NAME}")
add_library("${target}" MODULE)
target_sources("${target}" PRIVATE
  analyzer.cpp
  )
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${target}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries("${target}" src)
