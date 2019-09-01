#include "envtest.hpp"

void EnvTest::SetUp() {
  expect["heap"] = nullptr;
  expect["stack"] = nullptr;
}
