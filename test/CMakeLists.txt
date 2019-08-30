project(stacksafe-test CXX)

include(GoogleTest)

add_executable("${PROJECT_NAME}" example.cpp)
target_link_libraries("${PROJECT_NAME}" gtest_main)
gtest_add_tests(TARGET "${PROJECT_NAME}")
