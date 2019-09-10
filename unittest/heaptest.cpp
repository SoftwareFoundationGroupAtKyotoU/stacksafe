#include "heaptest.hpp"

void HeapTest::SetUp() { json = stacksafe::Json::object(); }
void HeapTest::equal() const {
  stacksafe::Json tmp = heap;
  EXPECT_EQ(json.dump(), tmp.dump());
}
