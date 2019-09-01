project(unittest CXX)

set(target ${PROJECT_NAME})
add_executable(${target})
target_sources(${target} PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources(${target})
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(${target}
  gtest_main
  src
  ${llvm_libs})

include(GoogleTest)
gtest_add_tests(TARGET ${target})
