project(stacksafe CXX)

# interface lib
set(interface "${PROJECT_NAME}-interface")
add_library("${interface}" INTERFACE)
## option
target_compile_features("${interface}" INTERFACE
  cxx_std_17)
target_compile_options("${interface}" INTERFACE
  -Wall -Wextra -pedantic)
## json
target_include_directories("${interface}" INTERFACE
  "${JSON_INCLUDE_DIR}")
## llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${interface}" INTERFACE
  "${LLVM_DEFINITIONS}")
target_include_directories("${interface}" INTERFACE
  "${LLVM_INCLUDE_DIRS}")
target_compile_options("${interface}" INTERFACE
  -fno-rtti)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries("${interface}" INTERFACE
  "${llvm_libs}")

# object lib
set(objects "${PROJECT_NAME}-objects")
add_library("${objects}" OBJECT
  analyzer.cpp
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
ensure_sources("${objects}")
set_target_properties("${objects}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
target_link_libraries("${objects}" "${interface}")

# module lib
set(module "${PROJECT_NAME}")
add_library("${module}" MODULE "$<TARGET_OBJECTS:${objects}>")
## target
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${module}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# archive lib
set(archive "${PROJECT_NAME}-archive")
add_library("${archive}" STATIC $<TARGET_OBJECTS:${objects}>)
target_link_libraries("${archive}" INTERFACE "${interface}")
