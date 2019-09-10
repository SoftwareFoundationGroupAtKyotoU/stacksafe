#include "heaptest.hpp"

TEST_F(HeapTest, Initial) { equal(); }

void HeapTest::SetUp() { json = stacksafe::Json::object(); }
void HeapTest::equal() const {
  stacksafe::Json tmp = heap;
  EXPECT_EQ(json.dump(), tmp.dump());
}
stacksafe::Symbol HeapTest::symbol() {
  return stacksafe::Symbol::create(stacksafe::Type{nullptr});
}
std::string HeapTest::sym(std::string a) { return "#" + a; }
