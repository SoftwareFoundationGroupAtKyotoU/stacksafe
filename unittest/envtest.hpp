#ifndef INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
#define INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45

#include <gtest/gtest.h>
#include "env.hpp"
#include "json.hpp"

struct EnvTest : ::testing::Test {
  stacksafe::Env env;
  stacksafe::Json expect;
};

#endif  // INCLUDE_GUARD_C7E72859_0086_4AA2_8E53_FAAE8ED32D45
