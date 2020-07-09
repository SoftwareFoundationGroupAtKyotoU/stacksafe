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
find_package(LLVM REQUIRED CONFIG)
## llvm interface
set(target llvm)
add_library(${target} INTERFACE)
target_compile_definitions(${target} INTERFACE
  ${LLVM_DEFINITIONS})
target_include_directories(${target} INTERFACE
  ${LLVM_INCLUDE_DIRS})
target_compile_options(${target} INTERFACE
  -fno-rtti -fPIC)
## llvm libs
set(target llvm-libs)
add_library(${target} INTERFACE IMPORTED)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(${target} INTERFACE
  ${llvm_libs})

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
## pass
add_subdirectory(src)
add_subdirectory(pass)
