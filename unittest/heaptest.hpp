#ifndef INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824
#define INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824

#include <gtest/gtest.h>
#include "json.hpp"
#include "map.hpp"

struct HeapTest : ::testing::Test {
  void equal() const;
  stacksafe::Heap heap;
  stacksafe::Json json;
};

#endif  // INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824
