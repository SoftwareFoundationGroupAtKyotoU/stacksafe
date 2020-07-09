cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)

# modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(BuildType)
include(CompileCommands)
include(DevelopFlag)
include(GitSubmodule)

# develop mode
add_custom_target(develop)
develop_flag_option(".develop")
if(DEVELOP_FLAG)
  set_build_type(Debug Release)
  export_compile_commands()
  add_dependencies(develop export-compile-commands)
else()
  set_build_type(Release Debug)
endif()
update_submodule()

# llvm
add_library(llvm INTERFACE)
find_package(LLVM REQUIRED CONFIG)
target_compile_definitions(llvm INTERFACE
  ${LLVM_DEFINITIONS})
target_include_directories(llvm INTERFACE
  ${LLVM_INCLUDE_DIRS})
target_compile_options(llvm INTERFACE
  -fno-rtti -fPIC)

# json
add_library(json INTERFACE)
target_compile_options(json INTERFACE
  -fno-exceptions)
target_link_libraries(json INTERFACE
  nlohmann_json::nlohmann_json)
set(BUILD_TESTING OFF)
set(JSON_BuildTests OFF)
add_subdirectory(json)

# sub directories
add_subdirectory(src)
add_subdirectory(pass)
