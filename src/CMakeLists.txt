project(stacksafe CXX)

set(archive "${PROJECT_NAME}-archive")
add_library("${archive}" STATIC)
## source
target_sources("${archive}" PRIVATE
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
ensure_sources("${archive}")
target_include_directories("${archive}" PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}")
## option
target_compile_features("${archive}" PUBLIC
  cxx_std_17)
target_compile_options("${archive}" PUBLIC
  -Wall -Wextra -pedantic)
set_target_properties("${archive}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
## json
target_include_directories("${archive}" PUBLIC
  "${JSON_INCLUDE_DIR}")
## llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${archive}" PUBLIC
  "${LLVM_DEFINITIONS}")
target_include_directories("${archive}" PUBLIC
  "${LLVM_INCLUDE_DIRS}")
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries("${archive}" INTERFACE
  "${llvm_libs}")
