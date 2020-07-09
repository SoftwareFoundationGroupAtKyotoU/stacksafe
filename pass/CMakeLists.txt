project(pass CXX)

set(CMAKE_SHARED_MODULE_PREFIX "")

add_library(stacksafe MODULE)
target_sources(stacksafe PRIVATE stacksafe.cpp)
set_target_properties(stacksafe PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries(stacksafe static)

add_library(dataflow MODULE)
target_sources(dataflow PRIVATE dataflow.cpp)
set_target_properties(dataflow PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries(dataflow static)
