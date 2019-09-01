cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(BuildType)
set_build_type()

include(CompileCommands)
export_compile_commands()

include(GitSubmodule)
update_submodule()

include(OverrideCache)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
add_subdirectory(googletest)
override_cache(INSTALL_GTEST OFF)
mark_as_advanced(FORCE BUILD_GMOCK INSTALL_GTEST)

# json
set(target nlohmann-json)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/json/include")

# llvm
find_package(LLVM REQUIRED CONFIG)
set(target llvm)
add_library(${target} INTERFACE)
target_compile_definitions(${target} INTERFACE
  ${LLVM_DEFINITIONS})
target_include_directories(${target} INTERFACE
  ${LLVM_INCLUDE_DIRS})
target_compile_options(${target} INTERFACE
  -fno-rtti -fPIC)

add_subdirectory(src)

add_subdirectory(pass)

enable_testing()
add_subdirectory(unittest)
