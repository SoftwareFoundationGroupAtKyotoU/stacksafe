project(stacksafe CXX)

# objects
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

# option
target_compile_features("${objects}" PRIVATE
  cxx_std_17)
target_compile_options("${objects}" PRIVATE
  -Wall -Wextra -pedantic)
set_target_properties("${objects}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)

# json
target_include_directories("${objects}" PRIVATE
  "${TOP_SOURCE_DIR}/json/include")

# llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${objects}" PRIVATE
  "${LLVM_DEFINITIONS}")
target_include_directories("${objects}" PRIVATE
  "${LLVM_INCLUDE_DIRS}")
target_compile_options("${objects}" PRIVATE
  -fno-rtti)

# target
add_library("${PROJECT_NAME}" MODULE)
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${PROJECT_NAME}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# source
target_sources("${PROJECT_NAME}" PRIVATE
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
ensure_sources()

# option
target_compile_features("${PROJECT_NAME}" PRIVATE
  cxx_std_17)
target_compile_options("${PROJECT_NAME}" PRIVATE
  -Wall -Wextra -pedantic)

# json
target_include_directories("${PROJECT_NAME}" PRIVATE
  "${TOP_SOURCE_DIR}/json/include")

# llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${PROJECT_NAME}" PRIVATE
  "${LLVM_DEFINITIONS}")
target_include_directories("${PROJECT_NAME}" PRIVATE
  "${LLVM_INCLUDE_DIRS}")
target_compile_options("${PROJECT_NAME}" PRIVATE
  -fno-rtti)
