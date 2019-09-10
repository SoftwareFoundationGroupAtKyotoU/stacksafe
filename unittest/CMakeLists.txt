project(unittest CXX)

set(target ${PROJECT_NAME})
add_executable(${target})
target_sources(${target} PRIVATE
  envtest.cpp
  heaptest.cpp
  testinst.cpp
  )
include(EnsureSources)
ensure_sources(${target})
target_link_libraries(${target}
  gtest_main
  llvm-libs
  src)

include(GoogleTest)
gtest_add_tests(TARGET ${target})
