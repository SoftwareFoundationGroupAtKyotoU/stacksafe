project(stacksafe-test CXX)

include(GoogleTest)

# test exe
add_executable("${PROJECT_NAME}")
target_sources("${PROJECT_NAME}" PRIVATE
  example.cpp
  allocainst.cpp
  )
include(EnsureSources)
ensure_sources("${PROJECT_NAME}")
target_include_directories("${PROJECT_NAME}" PRIVATE
  "${TOP_SOURCE_DIR}/src"
  "${JSON_INCLUDE_DIR}"
  )
target_link_libraries("${PROJECT_NAME}" PRIVATE
  gtest_main stacksafe-archive)
target_compile_features("${PROJECT_NAME}" PRIVATE
  cxx_std_17)
target_compile_options("${PROJECT_NAME}" PRIVATE
  -Wall -Wextra -pedantic)

gtest_add_tests(TARGET "${PROJECT_NAME}")
