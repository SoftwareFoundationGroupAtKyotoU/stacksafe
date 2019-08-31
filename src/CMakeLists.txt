project(stacksafe CXX)

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
## option
target_compile_features("${objects}" PRIVATE
  cxx_std_17)
target_compile_options("${objects}" PRIVATE
  -Wall -Wextra -pedantic)
set_target_properties("${objects}" PROPERTIES
  POSITION_INDEPENDENT_CODE ON)
## json
target_include_directories("${objects}" PRIVATE
  "${JSON_INCLUDE_DIR}")
## llvm
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions("${objects}" PRIVATE
  "${LLVM_DEFINITIONS}")
target_include_directories("${objects}" PRIVATE
  "${LLVM_INCLUDE_DIRS}")
target_compile_options("${objects}" PRIVATE
  -fno-rtti)

# module
set(module "${PROJECT_NAME}")
add_library("${module}" MODULE "$<TARGET_OBJECTS:${objects}>")
set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties("${module}" PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
