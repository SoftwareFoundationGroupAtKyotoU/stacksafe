project(src CXX)

set(target options)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${target} INTERFACE
  cxx_std_17)
target_compile_options(${target} INTERFACE
  -Wall -Wextra -pedantic)

set(target ${PROJECT_NAME})
add_library(${target} STATIC)
target_sources(${target} PRIVATE
  abstract.cpp
  debug.cpp
  domain.cpp
  env.cpp
  interpret.cpp
  json.cpp
  map.cpp
  type.cpp
  token.cpp
  )
include(EnsureSources)
ensure_sources(${target})
target_link_libraries(${target}
  llvm
  nlohmann-json
  options)
