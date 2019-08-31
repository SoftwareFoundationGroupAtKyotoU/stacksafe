project(tests CXX)

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources("${PROJECT_NAME}")
target_link_libraries("${PROJECT_NAME}"
  gtest_main archive)

include(GoogleTest)
gtest_add_tests(TARGET "${PROJECT_NAME}")
