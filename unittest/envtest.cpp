#include "envtest.hpp"

void EnvTest::SetUp() {
  expect["heap"] = Json::object_t{};
  expect["stack"] = Json::object_t{};
}
void EnvTest::init_heap(int key) { expect["heap"][sym(key)] = Json::array_t{}; }
void EnvTest::init_stack(int key) {
  expect["stack"][reg(key)] = Json::array_t{};
}
void EnvTest::push_heap(int key, int val) {
  expect["heap"][sym(key)].push_back(sym(val));
}
void EnvTest::push_stack(int key, int val) {
  expect["stack"][reg(key)].push_back(sym(val));
}
void EnvTest::equal() const {
  stacksafe::Json tmp = env;
  EXPECT_EQ(expect.dump(), tmp.dump());
}
std::string EnvTest::sym(int n) {
  using namespace std::string_literals;
  return "#"s + std::to_string(n);
}
std::string EnvTest::reg(int n) {
  using namespace std::string_literals;
  return "%"s + std::to_string(n);
}
