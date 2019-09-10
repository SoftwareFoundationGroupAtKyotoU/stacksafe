#ifndef INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824
#define INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824

#include <gtest/gtest.h>
#include <string>
#include "json.hpp"
#include "map.hpp"

struct HeapTest : ::testing::Test {
  using Domain = stacksafe::Domain;
  void SetUp();
  void init(std::string key);
  void push(std::string key, std::string val);
  void equal() const;
  static stacksafe::Symbol symbol();
  static std::string sym(std::string a);
  stacksafe::Heap heap;
  stacksafe::Json json;
};

#endif  // INCLUDE_GUARD_35398468_D0B7_4EC4_86F5_6DFBAD5C5824
