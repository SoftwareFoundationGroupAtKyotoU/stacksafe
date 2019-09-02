#include "envtest.hpp"

TEST_F(EnvTest, Alloc) {
  V v0{0}, v1{1};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
}

TEST_F(EnvTest, Store) {
  V v0{0}, v1{1};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
  env.store(v0, v1);
  push_heap(1, 0);
  equal();
}

TEST_F(EnvTest, Load) {
  V v0{0}, v1{1}, v2{2};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
  env.store(v0, v1);
  push_heap(1, 0);
  equal();
  env.load(v2, v1);
  push_stack(2, 0);
  equal();
}

TEST_F(EnvTest, BinOp) {
  V v0{0};
  equal();
  env.constant(v0);
  init_stack(0);
  equal();
}
