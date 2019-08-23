#include "collection.hpp"
#include <gtest/gtest.h>
#include <vector>

namespace {
struct OptRefTest : ::testing::Test {
  using Vec = std::vector<int>;
  using Ref = stacksafe::OptRef<Vec>;
  Vec v;
  Ref r, empty;
  OptRefTest() : v{{0, 1, 2, 3, 4}}, r{v} {}
  Ref &lvalue() { return r; }
  Ref &&rvalue() { return std::move(Ref{v}); }
  const Ref &clvalue() { return lvalue(); }
  const Ref &&crvalue() { return rvalue(); }
};
TEST_F(OptRefTest, OperatorBool) {
  EXPECT_EQ(static_cast<bool>(r), true);
  EXPECT_EQ(static_cast<bool>(empty), false);
}
TEST_F(OptRefTest, OperatorArrow) {
  ::testing::StaticAssertTypeEq<Vec *, decltype(lvalue().operator->())>();
  ::testing::StaticAssertTypeEq<Vec *, decltype(rvalue().operator->())>();
  ::testing::StaticAssertTypeEq<const Vec *,
                                decltype(clvalue().operator->())>();
  ::testing::StaticAssertTypeEq<const Vec *,
                                decltype(crvalue().operator->())>();
  EXPECT_EQ(r->front(), 0);
}
TEST_F(OptRefTest, OperatorAst) {
  ::testing::StaticAssertTypeEq<Vec &, decltype(lvalue().operator*())>();
  ::testing::StaticAssertTypeEq<Vec &&, decltype(rvalue().operator*())>();
  ::testing::StaticAssertTypeEq<const Vec &, decltype(clvalue().operator*())>();
  ::testing::StaticAssertTypeEq<const Vec &&,
                                decltype(crvalue().operator*())>();
  EXPECT_EQ(*r, v);
}
TEST_F(OptRefTest, Value) {
  ::testing::StaticAssertTypeEq<Vec &, decltype(lvalue().value())>();
  ::testing::StaticAssertTypeEq<Vec &&, decltype(rvalue().value())>();
  ::testing::StaticAssertTypeEq<const Vec &, decltype(clvalue().value())>();
  ::testing::StaticAssertTypeEq<const Vec &&, decltype(crvalue().value())>();
  EXPECT_EQ(r.value(), v);
}

struct OptCrefTest : ::testing::Test {
  using Vec = std::vector<int>;
  using Ref = stacksafe::OptRef<const Vec>;
  Vec v;
  Ref r, empty;
  OptCrefTest() : v{{0, 1, 2, 3, 4}}, r{v} {}
  Ref &lvalue() { return r; }
  Ref &&rvalue() { return std::move(Ref{v}); }
  const Ref &clvalue() { return lvalue(); }
  const Ref &&crvalue() { return rvalue(); }
};
TEST_F(OptCrefTest, OperatorBool) {
  EXPECT_EQ(static_cast<bool>(r), true);
  EXPECT_EQ(static_cast<bool>(empty), false);
}
TEST_F(OptCrefTest, OperatorArrow) {
  ::testing::StaticAssertTypeEq<const Vec *, decltype(lvalue().operator->())>();
  ::testing::StaticAssertTypeEq<const Vec *, decltype(rvalue().operator->())>();
  ::testing::StaticAssertTypeEq<const Vec *,
                                decltype(clvalue().operator->())>();
  ::testing::StaticAssertTypeEq<const Vec *,
                                decltype(crvalue().operator->())>();
  EXPECT_EQ(r->front(), 0);
}
TEST_F(OptCrefTest, OperatorAst) {
  ::testing::StaticAssertTypeEq<const Vec &, decltype(lvalue().operator*())>();
  ::testing::StaticAssertTypeEq<const Vec &&, decltype(rvalue().operator*())>();
  ::testing::StaticAssertTypeEq<const Vec &, decltype(clvalue().operator*())>();
  ::testing::StaticAssertTypeEq<const Vec &&,
                                decltype(crvalue().operator*())>();
  EXPECT_EQ(*r, v);
}
TEST_F(OptCrefTest, Value) {
  ::testing::StaticAssertTypeEq<const Vec &, decltype(lvalue().value())>();
  ::testing::StaticAssertTypeEq<const Vec &&, decltype(rvalue().value())>();
  ::testing::StaticAssertTypeEq<const Vec &, decltype(clvalue().value())>();
  ::testing::StaticAssertTypeEq<const Vec &&, decltype(crvalue().value())>();
  EXPECT_EQ(r.value(), v);
}
}  // namespace

#include "src/gtest_main.cc"
#include "visualize.cpp"
