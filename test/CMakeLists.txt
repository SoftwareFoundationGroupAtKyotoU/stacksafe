project(stacksafe-test CXX)

include(GoogleTest)

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources("${PROJECT_NAME}")
target_link_libraries("${PROJECT_NAME}" PRIVATE
  gtest_main stacksafe-archive)

gtest_add_tests(TARGET "${PROJECT_NAME}")
