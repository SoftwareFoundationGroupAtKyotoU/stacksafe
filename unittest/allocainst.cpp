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
  Json expect;
  auto alloc = [&expect](const std::string& sym, const std::string& reg) {
    expect["heap"][sym] = nullptr;
    expect["stack"][reg].push_back(sym);
  };
  expect["heap"] = nullptr;
  expect["stack"] = nullptr;
  EXPECT_EQ(expect.dump(), to_str(env));
  env.alloc(Value{0});
  alloc("#0", "%0");
  EXPECT_EQ(expect.dump(), to_str(env));
  env.alloc(Value{1});
  alloc("#1", "%1");
  EXPECT_EQ(expect.dump(), to_str(env));
}
