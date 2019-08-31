project(stacksafe CXX)

set(module "${CMAKE_PROJECT_NAME}")
add_library("${module}" MODULE)
target_sources("${module}" PRIVATE
  analyzer.cpp
  )
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${module}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries("${module}" archive)
