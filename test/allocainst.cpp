#include "env.hpp"
#include "gtest/gtest.h"
#include "json.hpp"

template <typename T>
std::string to_str(const T& t) {
  stacksafe::Json j = t;
  return j.dump();
}

TEST(AllocaTest, Single) {
  using namespace stacksafe;
  Env env;
  EXPECT_EQ(to_str(env), R"({"heap":null,"stack":null})");
}
