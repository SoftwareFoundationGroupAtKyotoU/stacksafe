cmake_minimum_required(VERSION 3.12.1)
project(top CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()

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

add_subdirectory(src)
add_subdirectory(test)
