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
TEST_F(HeapTest, InsertDomain) {
  stacksafe::Domain dom0, dom1;
  auto s0 = symbol(), s1 = symbol();
  auto a = "a", b = "b", c = "c", d = "d";
  dom0.insert(symbol());
  dom0.insert(symbol());
  dom1.insert(dom0);
  dom0.insert(s1);
  equal();
  heap.insert(s0, dom0);
  push(a, b);
  push(a, c);
  push(a, d);
  equal();
  heap.insert(s1, dom1);
  push(b, c);
  push(b, d);
  equal();
}

void HeapTest::SetUp() { json = stacksafe::Json::object(); }
void HeapTest::init(std::string key) {
  json[sym(key)] = stacksafe::Json::array();
}
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
