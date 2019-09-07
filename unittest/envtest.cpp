#include "envtest.hpp"

void EnvTest::SetUp() {
  expect["heap"] = Json::object_t{};
  expect["stack"] = Json::object_t{};
}
void EnvTest::init_heap(String key) {
  expect["heap"][sym(key)] = Json::array_t{};
}
void EnvTest::init_stack(int key) {
  expect["stack"][reg(key)] = Json::array_t{};
}
void EnvTest::push_heap(String key, String val) {
  expect["heap"][sym(key)].push_back(sym(val));
}
void EnvTest::push_stack(int key, String val) {
  expect["stack"][reg(key)].push_back(sym(val));
}
void EnvTest::equal() const {
  stacksafe::Json tmp = env;
  EXPECT_EQ(expect.dump(), tmp.dump());
}
auto EnvTest::sym(String a) -> String { return "#" + a; }
std::string EnvTest::reg(int n) { return "%" + std::to_string(n); }
