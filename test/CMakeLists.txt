project(stacksafe-test CXX)

include(GoogleTest)

add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  )

include(EnsureSources)
ensure_sources()

target_include_directories("${PROJECT_NAME}" PRIVATE
  "${TOP_SOURCE_DIR}/src"
  "${TOP_SOURCE_DIR}/json/include")

target_link_libraries("${PROJECT_NAME}" PRIVATE
  gtest_main)

target_compile_features("${PROJECT_NAME}" PRIVATE
  cxx_std_17)

gtest_add_tests(TARGET "${PROJECT_NAME}")
