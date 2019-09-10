#include "heaptest.hpp"

TEST_F(HeapTest, Initial) { equal(); }
TEST_F(HeapTest, InsertSymbol) {
  auto sa = symbol(), sb = symbol(), sc = symbol();
  auto a = "a", b = "b", c = "c";
  equal();
  heap.insert(sa, sb);
  push(a, b);
  equal();
  heap.insert(sa, sc);
  push(a, c);
  equal();
  heap.insert(sb, sc);
  push(b, c);
  equal();
}
TEST_F(HeapTest, InsertDomain) {
  stacksafe::Domain dom0, dom1;
  auto sa = symbol(), sb = symbol(), sc = symbol(), sd = symbol();
  auto a = "a", b = "b", c = "c", d = "d";
  dom0.insert(sc);
  dom0.insert(sd);
  dom1.insert(dom0);
  dom0.insert(sb);
  equal();
  heap.insert(sa, dom0);
  push(a, b);
  push(a, c);
  push(a, d);
  equal();
  heap.insert(sb, dom1);
  push(b, c);
  push(b, d);
  equal();
}
TEST_F(HeapTest, InsertEmpty) {
  auto sa = symbol(), sb = symbol();
  auto a = "a", b = "b";
  equal();
  heap.insert(sa);
  init(a);
  equal();
  heap.insert(sb);
  init(b);
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
