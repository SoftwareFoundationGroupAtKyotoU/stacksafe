cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(TopDir)

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

set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/json/include")

add_subdirectory(src)

add_subdirectory(stacksafe)

enable_testing()
add_subdirectory(test)
