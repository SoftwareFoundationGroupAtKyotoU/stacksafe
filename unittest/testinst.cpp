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

TEST_F(EnvTest, Constant) {
  V v0{0};
  equal();
  env.constant(v0);
  init_stack(0);
  equal();
}

TEST_F(EnvTest, Call) {
  V v0{0}, v1{1}, v2{2}, v3{3};
  equal();
  env.alloc(v0);
  init_heap(0);
  push_stack(0, 0);
  equal();
  env.alloc(v1);
  init_heap(1);
  push_stack(1, 1);
  equal();
  env.alloc(v2);
  init_heap(2);
  push_stack(2, 2);
  equal();
  env.store(v1, v0);
  push_heap(0, 1);
  equal();
  env.store(v2, v0);
  push_heap(0, 2);
  equal();
  Params params{v0, v1};
  env.call(v3, params);
  auto &key0 = expect["heap"][sym(0)];
  key0.insert(key0.begin(), sym(0));
  for (int i = 0; i < 3; ++i) {
    push_heap(1, i);
    push_heap(2, i);
    push_stack(3, i);
  }
  equal();
}
