#include "envtest.hpp"

void EnvTest::SetUp() {
  expect["heap"] = nullptr;
  expect["stack"] = nullptr;
}
std::string EnvTest::sym(int n) {
  using namespace std::string_literals;
  return "#"s + std::to_string(n);
}
std::string EnvTest::reg(int n) {
  using namespace std::string_literals;
  return "%"s + std::to_string(n);
}
