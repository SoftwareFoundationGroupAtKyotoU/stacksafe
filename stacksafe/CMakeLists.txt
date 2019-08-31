project(stacksafe CXX)

add_library("${PROJECT_NAME}" MODULE)
target_sources("${PROJECT_NAME}" PRIVATE
  analyzer.cpp
  )
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${PROJECT_NAME}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries("${PROJECT_NAME}" "${PROJECT_NAME}-archive")
