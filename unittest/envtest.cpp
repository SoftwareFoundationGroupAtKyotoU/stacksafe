#include "envtest.hpp"

void EnvTest::SetUp() {
  expect["heap"] = nullptr;
  expect["stack"] = nullptr;
}
void EnvTest::init_heap(int key) { expect["heap"][sym(key)] = nullptr; }
void EnvTest::init_stack(int key) { expect["stack"][reg(key)] = nullptr; }
std::string EnvTest::sym(int n) {
  using namespace std::string_literals;
  return "#"s + std::to_string(n);
}
std::string EnvTest::reg(int n) {
  using namespace std::string_literals;
  return "%"s + std::to_string(n);
}
