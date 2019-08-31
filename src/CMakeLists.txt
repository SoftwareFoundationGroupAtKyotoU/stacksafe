project(stacksafe CXX)

# interface lib
set(interface "${PROJECT_NAME}-interface")
add_library("${interface}" INTERFACE)
## option
target_compile_features("${interface}" INTERFACE
  cxx_std_17)
target_compile_options("${interface}" INTERFACE
  -Wall -Wextra -pedantic)
## include
target_include_directories("${interface}" INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}")
## json
target_include_directories("${interface}" INTERFACE
  "${JSON_INCLUDE_DIR}")
## llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${interface}" INTERFACE
  "${LLVM_DEFINITIONS}")
target_include_directories("${interface}" INTERFACE
  "${LLVM_INCLUDE_DIRS}")
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries("${interface}" INTERFACE
  "${llvm_libs}")

# archive lib
set(archive "${PROJECT_NAME}-archive")
add_library("${archive}" STATIC)
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
set_target_properties("${archive}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
target_link_libraries("${archive}" "${interface}")
