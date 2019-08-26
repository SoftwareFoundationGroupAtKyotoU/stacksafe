cmake_minimum_required(VERSION 3.12.1)
project(stacksafe CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(BuildType)
set_build_type()

include(DownloadGoogleTest)
download_googletest()

add_subdirectory(src)
add_subdirectory(test)
