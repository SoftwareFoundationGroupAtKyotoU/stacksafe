#include "heaptest.hpp"

TEST_F(HeapTest, Initial) { equal(); }
TEST_F(HeapTest, InsertSymbol) {
  auto s0 = symbol(), s1 = symbol(), s2 = symbol();
  auto a = "a", b = "b", c = "c";
  equal();
  heap.insert(s0, s1);
  push(a, b);
  equal();
  heap.insert(s0, s2);
  push(a, c);
  equal();
  heap.insert(s1, s2);
  push(b, c);
  equal();
}

void HeapTest::SetUp() { json = stacksafe::Json::object(); }
void HeapTest::push(std::string key, std::string val) {
  json[sym(key)].push_back(sym(val));
}
void HeapTest::equal() const {
  stacksafe::Json tmp = heap;
  EXPECT_EQ(json.dump(), tmp.dump());
}
stacksafe::Symbol HeapTest::symbol() {
  return stacksafe::Symbol::create(stacksafe::Type{nullptr});
}
std::string HeapTest::sym(std::string a) { return "#" + a; }
