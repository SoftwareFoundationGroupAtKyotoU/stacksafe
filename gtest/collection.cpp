#include "gtest/gtest.h"
#include "collection.hpp"

namespace {
  TEST(OptRefTest, IntOptRef) {
    int x = 42;
    stacksafe::OptRef<int> ref{x};
    EXPECT_EQ(static_cast<bool>(ref), true);
  }
}

#include "visualize.cpp"
#include "src/gtest_main.cc"
