add_library(${PROJECT_NAME} MODULE
  analyzer.cpp
  interpret.cpp
  symbol.cpp
  )

find_package(LLVM REQUIRED CONFIG)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${LLVM_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE
  ${LLVM_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE
  -fno-rtti)

set(CMAKE_SHARED_MODULE_PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE
  cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall -Wextra -pedantic)
