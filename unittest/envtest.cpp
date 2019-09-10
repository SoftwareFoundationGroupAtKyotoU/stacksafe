#include "envtest.hpp"

TEST_F(EnvTest, Binop) {
  equal();
  env.binop(v0, v1, v2);
  init_stack(0);
  equal();
}
TEST_F(EnvTest, Alloc) {
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
}
TEST_F(EnvTest, Load) {
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.store(v0, v1);
  push_heap(b, a);
  equal();
  env.load(v2, v1);
  push_stack(2, a);
  equal();
}
TEST_F(EnvTest, Store) {
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.store(v0, v1);
  push_heap(b, a);
  equal();
}
TEST_F(EnvTest, Cast) {
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.cast(v1, v0);
  push_stack(1, a);
  equal();
}
TEST_F(EnvTest, Call) {
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.alloc(v2);
  init_heap(c);
  push_stack(2, c);
  equal();
  env.store(v1, v0);
  push_heap(a, b);
  equal();
  env.store(v2, v0);
  push_heap(a, c);
  equal();
  Params params{v0, v1};
  env.call(v3, params);
  auto &key0 = expect["heap"][sym(a)];
  key0.insert(key0.begin(), sym(a));
  for (auto i : {a, b, c}) {
    push_heap(b, i);
    push_heap(c, i);
    push_stack(3, i);
  }
  equal();
}
TEST_F(EnvTest, Constant) {
  equal();
  env.constant(v0);
  init_stack(0);
  equal();
}

void EnvTest::SetUp() {
  expect["heap"] = Json::object();
  expect["stack"] = Json::object();
}
void EnvTest::init_heap(std::string key) {
  expect["heap"][sym(key)] = Json::array();
}
void EnvTest::init_stack(int key) { expect["stack"][reg(key)] = Json::array(); }
void EnvTest::push_heap(std::string key, std::string val) {
  expect["heap"][sym(key)].push_back(sym(val));
}
void EnvTest::push_stack(int key, std::string val) {
  expect["stack"][reg(key)].push_back(sym(val));
}
void EnvTest::equal() const {
  Json tmp = env;
  EXPECT_EQ(expect.dump(), tmp.dump());
}
std::string EnvTest::sym(std::string a) { return "#" + a; }
std::string EnvTest::reg(int n) { return "%" + std::to_string(n); }
const stacksafe::Value EnvTest::v0{0};
const stacksafe::Value EnvTest::v1{1};
const stacksafe::Value EnvTest::v2{2};
const stacksafe::Value EnvTest::v3{3};
const EnvTest::String EnvTest::a = "a";
const EnvTest::String EnvTest::b = "b";
const EnvTest::String EnvTest::c = "c";
