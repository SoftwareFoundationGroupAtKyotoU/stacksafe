project(src CXX)

add_library(options INTERFACE)
target_include_directories(options INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(options INTERFACE
  cxx_std_17)
target_compile_options(options INTERFACE
  $<$<CONFIG:Debug>:-pedantic -Wall -Wextra -Wno-unused-parameter>)

add_library(static STATIC)
target_sources(static PRIVATE
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
ensure_sources(static)
target_link_libraries(static
  json
  llvm
  options)
