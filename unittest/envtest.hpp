#ifndef INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
#define INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45

#include <gtest/gtest.h>
#include <string>
#include "env.hpp"
#include "json.hpp"

struct EnvTest : ::testing::Test {
  virtual void SetUp() override;
  void init_heap(int key);
  void init_stack(int key);
  void push_heap(int key, int val);
  void push_stack(int key, int val);
  void equal() const;
  static std::string sym(int n);
  static std::string reg(int n);
  stacksafe::Env env;
  stacksafe::Json expect;
};

#endif  // INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
