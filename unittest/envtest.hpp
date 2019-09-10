#ifndef INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
#define INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45

#include <gtest/gtest.h>
#include <string>
#include "env.hpp"
#include "json.hpp"

struct EnvTest : ::testing::Test {
  using Json = stacksafe::Json;
  using Params = stacksafe::Params;
  using String = std::string;
  virtual void SetUp() override;
  void init_heap(String key);
  void init_stack(int key);
  void push_heap(String key, String val);
  void push_stack(int key, String val);
  void equal() const;
  static String sym(String a);
  static std::string reg(int n);
  stacksafe::Env env;
  Json expect;
  static const stacksafe::Value v0, v1, v2, v3;
  static const char *const a, *const b, *const c;
};

#endif  // INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
