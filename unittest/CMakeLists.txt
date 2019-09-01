project(unittest CXX)

set(target ${PROJECT_NAME})
add_executable(${target})
target_sources(${target} PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources(${target})
target_link_libraries(${target}
  gtest_main
  src
  llvm-libs)

include(GoogleTest)
gtest_add_tests(TARGET ${target})
