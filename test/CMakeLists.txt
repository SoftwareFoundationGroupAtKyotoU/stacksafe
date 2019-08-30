project(stacksafe-test CXX)

include(GoogleTest)

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  )

include(EnsureSources)
ensure_sources()

target_link_libraries("${PROJECT_NAME}" gtest_main)
gtest_add_tests(TARGET "${PROJECT_NAME}")
