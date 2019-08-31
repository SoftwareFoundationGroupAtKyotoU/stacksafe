project(unittest CXX)

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources("${PROJECT_NAME}")
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries("${PROJECT_NAME}"
  gtest_main
  archive
  "${llvm_libs}")

include(GoogleTest)
gtest_add_tests(TARGET "${PROJECT_NAME}")
