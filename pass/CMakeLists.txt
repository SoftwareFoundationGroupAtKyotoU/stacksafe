project(pass CXX)

set(CMAKE_SHARED_MODULE_PREFIX "")

set(target ${CMAKE_PROJECT_NAME})
add_library(${target} MODULE)
target_sources(${target} PRIVATE
  stacksafe.cpp
  )
set_target_properties(${target} PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries(${target}
  src)

add_library(dataflow MODULE)
target_sources(dataflow PRIVATE dataflow.cpp)
set_target_properties(dataflow PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries(dataflow src)
