#include "envtest.hpp"

TEST_F(EnvTest, Alloc) {
  V v0{0}, v1{1};
  const auto a = "a", b = "b";
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
}

TEST_F(EnvTest, Store) {
  V v0{0}, v1{1};
  const auto a = "a", b = "b";
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.store(v0, v1);
  push_heap(b, a);
  equal();
}

TEST_F(EnvTest, Load) {
  V v0{0}, v1{1}, v2{2};
  const auto a = "a", b = "b";
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.store(v0, v1);
  push_heap(b, a);
  equal();
  env.load(v2, v1);
  push_stack(2, a);
  equal();
}

TEST_F(EnvTest, Constant) {
  V v0{0};
  equal();
  env.constant(v0);
  init_stack(0);
  equal();
}

TEST_F(EnvTest, Call) {
  V v0{0}, v1{1}, v2{2}, v3{3};
  const auto a = "a", b = "b", c = "c";
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.alloc(v1);
  init_heap(b);
  push_stack(1, b);
  equal();
  env.alloc(v2);
  init_heap(c);
  push_stack(2, c);
  equal();
  env.store(v1, v0);
  push_heap(a, b);
  equal();
  env.store(v2, v0);
  push_heap(a, c);
  equal();
  Params params{v0, v1};
  env.call(v3, params);
  auto &key0 = expect["heap"][sym(a)];
  key0.insert(key0.begin(), sym(a));
  for (auto i : {a, b, c}) {
    push_heap(b, i);
    push_heap(c, i);
    push_stack(3, i);
  }
  equal();
}

TEST_F(EnvTest, Cast) {
  V v0{0}, v1{1};
  const auto a = "a";
  equal();
  env.alloc(v0);
  init_heap(a);
  push_stack(0, a);
  equal();
  env.cast(v1, v0);
  push_stack(1, a);
  equal();
}
