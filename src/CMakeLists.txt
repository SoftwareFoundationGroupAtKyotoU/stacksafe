project(archive CXX)

set(target "${PROJECT_NAME}")
add_library("${target}" STATIC)
# source
target_sources("${target}" PRIVATE
  domain.cpp
  env.cpp
  interpret.cpp
  json.cpp
  map.cpp
  symbol.cpp
  type.cpp
  value.cpp
  )
include(EnsureSources)
ensure_sources("${target}")
target_include_directories("${target}" PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}")
# option
target_compile_features("${target}" PUBLIC
  cxx_std_17)
target_compile_options("${target}" PUBLIC
  -Wall -Wextra -pedantic)
set_target_properties("${target}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
# json
target_include_directories("${target}" PUBLIC
  "${JSON_INCLUDE_DIR}")
# llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${target}" PUBLIC
  "${LLVM_DEFINITIONS}")
target_include_directories("${target}" PUBLIC
  "${LLVM_INCLUDE_DIRS}")
target_compile_options("${target}" PUBLIC
  -fno-rtti)
