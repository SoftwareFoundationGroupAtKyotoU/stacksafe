cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(BuildType)
set_build_type()

include(CompileCommands)
export_compile_commands()

include(GitSubmodule)
update_submodule()

# googletest
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
add_subdirectory(googletest)
include(OverrideCache)
override_cache(INSTALL_GTEST OFF)
mark_as_advanced(FORCE BUILD_GMOCK INSTALL_GTEST)
set(target googletest)
add_library(${target} INTERFACE IMPORTED)
target_link_libraries(${target} INTERFACE
  gtest_main)

# json
set(target nlohmann-json)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/json/include")

find_package(LLVM REQUIRED CONFIG)
# llvm
set(target llvm)
add_library(${target} INTERFACE)
target_compile_definitions(${target} INTERFACE
  ${LLVM_DEFINITIONS})
target_include_directories(${target} INTERFACE
  ${LLVM_INCLUDE_DIRS})
target_compile_options(${target} INTERFACE
  -fno-rtti -fPIC)
# llvm libs
set(target llvm-libs)
add_library(${target} INTERFACE IMPORTED)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(${target} INTERFACE
  ${llvm_libs})

add_subdirectory(src)

add_subdirectory(pass)

enable_testing()
add_subdirectory(unittest)
