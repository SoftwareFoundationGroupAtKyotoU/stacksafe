#ifndef INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
#define INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45

#include <gtest/gtest.h>
#include <string>
#include "env.hpp"
#include "json.hpp"
#include "token.hpp"

struct EnvTest : ::testing::Test {
  using Json = stacksafe::Json;
  using Params = stacksafe::Params;
  using String = const char *;
  virtual void SetUp() override;
  void init_heap(std::string key);
  void init_stack(int key);
  void push_heap(std::string key, std::string val);
  void push_stack(int key, std::string val);
  void equal() const;
  static std::string sym(std::string a);
  static std::string reg(int n);
  stacksafe::Memory env;
  Json expect;
  static const stacksafe::Value v0, v1, v2, v3;
  static const String a, b, c;
};

#endif  // INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
