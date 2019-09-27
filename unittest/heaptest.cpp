#include "heaptest.hpp"

TEST_F(HeapTest, Initial) {
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
TEST_F(HeapTest, InsertMap) {
  stacksafe::Heap heap0, heap1;
  auto sa = symbol(), sb = symbol(), sc = symbol();
  auto a = "a", b = "b", c = "c";
  heap0.insert(sa, Domain{sb});
  heap0.insert(sb, Domain{sc});
  heap1.insert(sc, Domain{sa, sb});
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
  heap.insert(sa, Domain{});
  CHECK_TRUE();
  sub.insert(sb, Domain{});
  CHECK_FALSE();
  sub.insert(sa, Domain{sb});
  CHECK_FALSE();
  heap.insert(sb, Domain{sc});
  CHECK_FALSE();
  heap.insert(sa, Domain{sb});
  CHECK_TRUE();
  heap.insert(sa, Domain{sc});
  CHECK_TRUE();
  sub.insert(sc, Domain{sa});
  CHECK_FALSE();
  heap.insert(sc, Domain{});
  CHECK_FALSE();
  sub.insert(sb, Domain{sc});
  CHECK_FALSE();
  heap.insert(sub);
  CHECK_TRUE();
#undef CHECK_TRUE
#undef CHECK_FALSE
}
TEST_F(HeapTest, Get) {
  Domain dom;
  auto sa = symbol(), sb = symbol(), sc = symbol();
#define CHECK_EQ(key)                        \
  ASSERT_NE(nullptr, heap.get(key));         \
  EXPECT_TRUE(dom.includes(*heap.get(key))); \
  EXPECT_TRUE(heap.get(key)->includes(dom))
  EXPECT_EQ(nullptr, heap.get(sa));
  EXPECT_EQ(nullptr, heap.get(sb));
  heap.insert(sa, Domain{});
  EXPECT_NE(nullptr, heap.get(sa));
  EXPECT_EQ(nullptr, heap.get(sb));
  CHECK_EQ(sa);
  dom = Domain{sa, sb};
  heap.insert(sa, dom);
  EXPECT_NE(nullptr, heap.get(sa));
  EXPECT_EQ(nullptr, heap.get(sb));
  CHECK_EQ(sa);
  dom = Domain{sa, sc};
  heap.insert(sb, dom);
  EXPECT_NE(nullptr, heap.get(sa));
  EXPECT_NE(nullptr, heap.get(sb));
  CHECK_EQ(sb);
#undef CHECK_EQ
}

void HeapTest::SetUp() {
  stacksafe::Symbol::reset();
  json = stacksafe::Json::object();
}
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
  return stacksafe::Symbol::make(stacksafe::Type{nullptr});
}
std::string HeapTest::sym(std::string a) {
  return "#" + a;
}
