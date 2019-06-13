#include "collection.hpp"
#include <gtest/gtest.h>

namespace {
struct OptRefTest : ::testing::Test {
  int x;
  stacksafe::OptRef<int> empty, null, value;
  OptRefTest() : x{42}, empty{}, null{std::nullopt}, value{x} {}
};
TEST_F(OptRefTest, OperatorBool) {
  EXPECT_EQ(static_cast<bool>(empty), false);
  EXPECT_EQ(static_cast<bool>(null), false);
  EXPECT_EQ(static_cast<bool>(value), true);
}
} // namespace

#include "src/gtest_main.cc"
#include "visualize.cpp"
