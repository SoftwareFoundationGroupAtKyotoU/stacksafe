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
  auto sa = symbol(), sb = symbol(), sc = symbol();
  auto a = "a", b = "b", c = "c";
  dom0.insert(sc);
  dom1.insert(dom0);
  dom0.insert(sb);
  equal();
  heap.insert(sa, dom0);
  push(a, b);
  push(a, c);
  equal();
  heap.insert(sb, dom1);
  push(b, c);
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
TEST_F(HeapTest, InsertMap) {
  stacksafe::Heap heap0, heap1;
  auto sa = symbol(), sb = symbol(), sc = symbol();
  auto a = "a", b = "b", c = "c";
  heap0.insert(sa, sb);
  heap0.insert(sb, sc);
  heap1.insert(sc, sa);
  heap1.insert(sc, sb);
  equal();
  heap.insert(heap0);
  push(a, b);
  push(b, c);
  equal();
  heap.insert(heap1);
  push(c, a);
  push(c, b);
  equal();
}
TEST_F(HeapTest, Includes) {
  stacksafe::Heap sub;
  auto sa = symbol(), sb = symbol(), sc = symbol();
#define CHECK_TRUE() EXPECT_TRUE(heap.includes(sub))
#define CHECK_FALSE() EXPECT_FALSE(heap.includes(sub))
  CHECK_TRUE();
  heap.insert(sa);
  CHECK_TRUE();
  sub.insert(sb);
  CHECK_FALSE();
  sub.insert(sa, sb);
  CHECK_FALSE();
  heap.insert(sb, sc);
  CHECK_FALSE();
  heap.insert(sa, sb);
  CHECK_TRUE();
  heap.insert(sa, sc);
  CHECK_TRUE();
  sub.insert(sc, sa);
  CHECK_FALSE();
  heap.insert(sc);
  CHECK_FALSE();
  sub.insert(sb, sc);
  CHECK_FALSE();
  heap.insert(sub);
  CHECK_TRUE();
#undef CHECK_TRUE
#undef CHECK_FALSE
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
