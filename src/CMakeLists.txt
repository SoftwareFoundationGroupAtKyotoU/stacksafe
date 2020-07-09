project(src CXX)

set(target options)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${target} INTERFACE
  cxx_std_17)
target_compile_options(${target} INTERFACE
  $<$<CONFIG:Debug>:-pedantic -Wall -Wextra -Wno-unused-parameter>)

set(target ${PROJECT_NAME})
add_library(${target} STATIC)
target_sources(${target} PRIVATE
  block.cpp
  cache.cpp
  component.cpp
  effect.cpp
  function.cpp
  graph.cpp
  matrix.cpp
  node.cpp
  params.cpp
  pointsto.cpp
  state.cpp
  stopwatch.cpp
  tarjan.cpp
  tree.cpp
  utility.cpp
  )
include(EnsureSources)
ensure_sources(${target})
target_link_libraries(${target}
  llvm
  nlohmann_json::nlohmann_json
  options)
