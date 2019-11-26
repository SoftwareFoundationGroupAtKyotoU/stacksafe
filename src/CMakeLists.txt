project(src CXX)

set(target options)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${target} INTERFACE
  cxx_std_17)
target_compile_options(${target} INTERFACE
  $<$<CONFIG:Debug>:-Wall -Wextra -pedantic>)

set(target ${PROJECT_NAME})
add_library(${target} STATIC)
target_sources(${target} PRIVATE
  abstract.cpp
  base.cpp
  cache.cpp
  domain.cpp
  error.cpp
  graph.cpp
  hash.cpp
  interpreter.cpp
  json.cpp
  log.cpp
  map.cpp
  params.cpp
  stopwatch.cpp
  utility.cpp
  value.cpp
  )
include(EnsureSources)
ensure_sources(${target})
target_link_libraries(${target}
  llvm
  nlohmann-json
  options)
