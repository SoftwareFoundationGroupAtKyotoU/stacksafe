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
  enable_testing()
  export_compile_commands()
  add_dependencies(develop export-compile-commands)
else()
  set_build_type(Release Debug)
endif()

# submodules
update_submodule()
## googletest
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest." OFF)
mark_as_advanced(FORCE BUILD_GMOCK INSTALL_GTEST)
add_subdirectory(googletest EXCLUDE_FROM_ALL)
## json
set(target nlohmann-json)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/json/include")

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

# sub directories
## pass
add_subdirectory(src)
add_subdirectory(pass)
## test
add_subdirectory(unittest EXCLUDE_FROM_ALL)
add_dependencies(develop unittest)
