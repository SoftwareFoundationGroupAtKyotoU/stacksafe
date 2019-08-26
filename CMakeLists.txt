cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(BuildType)
set_build_type()

enable_testing()

include(CompileCommands)
export_compile_commands()

include(GitSubmodule)
update_submodule()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(googletest)
include_directories(json/include)
