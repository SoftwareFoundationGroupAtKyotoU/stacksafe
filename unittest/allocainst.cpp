#include "envtest.hpp"

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

TEST(StoreTest, AllocStore) {
  using namespace stacksafe;
  Env env;
  Json expect;
  Value v0{0}, v1{1};
  expect["heap"] = nullptr;
  expect["stack"] = nullptr;
  EXPECT_EQ(expect.dump(), to_str(env));
  env.alloc(v0);
  expect["heap"]["#0"] = nullptr;
  expect["stack"]["%0"].push_back("#0");
  EXPECT_EQ(expect.dump(), to_str(env));
  env.alloc(v1);
  expect["heap"]["#1"] = nullptr;
  expect["stack"]["%1"].push_back("#1");
  EXPECT_EQ(expect.dump(), to_str(env));
  env.store(v0, v1);
  expect["heap"]["#1"].push_back("#0");
  EXPECT_EQ(expect.dump(), to_str(env));
}

TEST_F(EnvTest, Alloc) {
  V v0{0}, v1{1};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
}

TEST_F(EnvTest, Store) {
  V v0{0}, v1{1};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
  env.store(v0, v1);
  push_heap(1, 0);
  equal();
}
