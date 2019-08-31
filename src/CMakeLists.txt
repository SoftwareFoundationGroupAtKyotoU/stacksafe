project(archive CXX)

add_library("${PROJECT_NAME}" STATIC)
# source
target_sources("${PROJECT_NAME}" PRIVATE
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
ensure_sources("${PROJECT_NAME}")
target_include_directories("${PROJECT_NAME}" PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}")
# option
target_compile_features("${PROJECT_NAME}" PUBLIC
  cxx_std_17)
target_compile_options("${PROJECT_NAME}" PUBLIC
  -Wall -Wextra -pedantic)
set_target_properties("${PROJECT_NAME}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
# json
target_include_directories("${PROJECT_NAME}" PUBLIC
  "${JSON_INCLUDE_DIR}")
# llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${PROJECT_NAME}" PUBLIC
  "${LLVM_DEFINITIONS}")
target_include_directories("${PROJECT_NAME}" PUBLIC
  "${LLVM_INCLUDE_DIRS}")
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries("${PROJECT_NAME}" INTERFACE
  "${llvm_libs}")
