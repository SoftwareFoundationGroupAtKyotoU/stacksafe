include(DownloadGoogleTest)
download_googletest()

include(GoogleTest)

add_executable(example example.cpp)
target_link_libraries(example gtest_main)
gtest_add_tests(TARGET example)
